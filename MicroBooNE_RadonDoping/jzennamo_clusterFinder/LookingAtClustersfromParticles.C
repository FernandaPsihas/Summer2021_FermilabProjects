/// Psuedo code:
/*

  Go through all the clusters then associates 
  hit and MC particle information associated with them
    
*/


// Standard things to include
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include <fstream>
#include <iterator>
#include <algorithm>
#include <math.h> 
// These are the includes to use "Root" things 
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

// These are the larsoft includes that let you
// have access to data-products and the event 
// details
#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"
#include "gallery/ValidHandle.h"
#include "gallery/Handle.h"
#include "canvas/Persistency/Common/FindMany.h"
#include "canvas/Persistency/Common/FindOne.h"
#include "canvas/Persistency/Common/FindManyP.h"
#include "canvas/Persistency/Common/fwd.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "canvas/Persistency/Provenance/Timestamp.h"

//I'll only need truth particle info for now:
#include "nusimdata/SimulationBase/MCParticle.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/AnalysisBase/BackTrackerMatchingData.h"

//This way you can be lazy
using namespace art;
using namespace std;

//This Block Lets us convert a TGraph with 
// a free normalization term
// Moving forward this is the function that will be fit to
// our reconstructed energy distributions so we'll 
// modify this to include terms that could smear this 
// data out
TGraph *BetaData;
double FittedFunction(double *xx, double *par)
{
  return par[0]*BetaData->Eval(xx[0]);
}

void LookingAtClustersfromParticles(){

  // create a vector of files we want to process
  //  std::vector<std::string> filenames;
  
  // read in a file list that we get from a sam-def but remember it 
  // is very long so if we want to run over it all it'll take a while
  // and we'll probably want to break it up on the grid
  //ifstream myfile("my_filelist.list");
  //copy(istream_iterator<string>(myfile),
  //     istream_iterator<string>(),
  //     back_inserter(filenames));
  
  //We'll just check the first 10k files for now
  //filenames.erase(filenames.begin()+10000,filenames.end());
  
  //Here I just hard coding a file for testing, we can adjust this later 
  vector<string> filenames {"/pnfs/uboone/persistent/users/ilepetic/newoverlays/ilepetic_gamma3d_newoverlays/21080126_108/output.root"};
  
  // Here we will create all of our histograms 

  //We can also make a file that contains a multidimensional
  //break down for our data, we'll build this for each beta 
  TFile* out = new TFile("outfile_mc_TEST.root","RECREATE");  
  TTree* fTree = new TTree("ClusterTree","All information saved by cluster");
  
  //Cluster info
  std::vector<double> cluster_x_vec;
  std::vector<double> cluster_z_vec;
  std::vector<double> cluster_start_wire;
  std::vector<double> cluster_end_wire;
  std::vector<double> cluster_energy;
  std::vector<double> cluster_avg_energy; //IntegralAverage()
  std::vector<double> cluster_std_energy; //IntegralStdDev()
  std::vector<double> cluster_multi_hit_density; //MultipleHitDensity()
 
  std::vector<bool> cluster_IsTrueBeta_vec;
  std::vector<bool> cluster_IsTrueAlpha_vec;
  std::vector<bool> cluster_MatchedToMCpart_vec;
  
  fTree->Branch("cluster_x_vec",&cluster_x_vec);
  fTree->Branch("cluster_z_vec",&cluster_z_vec);
  fTree->Branch("cluster_start_wire",&cluster_start_wire);
  fTree->Branch("cluster_end_wire",&cluster_end_wire);
  fTree->Branch("cluster_energy",&cluster_energy);
  fTree->Branch("cluster_avg_energy",&cluster_avg_energy);
  fTree->Branch("cluster_std_energy",&cluster_std_energy);
  fTree->Branch("cluster_multi_hit_density",&cluster_multi_hit_density);
  fTree->Branch("cluster_IsTrueBeta_vec",&cluster_IsTrueBeta_vec);
  fTree->Branch("cluster_IsTrueAlpha_vec",&cluster_IsTrueAlpha_vec);
  fTree->Branch("cluster_MatchedToMCpart_vec",&cluster_MatchedToMCpart_vec);

  //Hit info
  std::vector< std::vector< double> > hit_Q_for_cluster_vec;//Integral
  std::vector< std::vector< double> > hit_sigma_for_cluster_vec;//RMS
  std::vector< std::vector< double> > hit_GOF_for_cluster_vec;//GoodnessOfFit
  std::vector< std::vector< double> > hit_mult_for_cluster_vec;//Multiplicity

  fTree->Branch("hit_Q_for_cluster_vec",&hit_Q_for_cluster_vec);
  fTree->Branch("hit_sigma_for_cluster_vec",&hit_sigma_for_cluster_vec);
  fTree->Branch("hit_GOF_for_cluster_vec",&hit_GOF_for_cluster_vec);
  fTree->Branch("hit_mult_for_cluster_vec",&hit_mult_for_cluster_vec);

  //MCPart info  
  std::vector< std::vector< std::vector< double > > > mcpart_pdg_for_hit_for_cluster_vec;
  std::vector< std::vector< std::vector< double > > > mcpart_primary_for_hit_for_cluster_vec;

  std::vector< std::vector< std::vector< double > > > mcpart_trueX_for_hit_for_cluster_vec;
  std::vector< std::vector< std::vector< double > > > mcpart_trueY_for_hit_for_cluster_vec;
  std::vector< std::vector< std::vector< double > > > mcpart_trueZ_for_hit_for_cluster_vec;

  std::vector< std::vector< std::vector< double > > > mcpart_frac_for_hit_for_cluster_vec;
  std::vector< std::vector< std::vector< double > > > mcpart_matched_energy_for_hit_for_cluster_vec;

  fTree->Branch("mcpart_pdg_for_hit_for_cluster_vec",&mcpart_pdg_for_hit_for_cluster_vec);
  fTree->Branch("mcpart_primary_for_hit_for_cluster_vec",&mcpart_primary_for_hit_for_cluster_vec);

  fTree->Branch("mcpart_trueX_for_hit_for_cluster_vec",&mcpart_trueX_for_hit_for_cluster_vec);
  fTree->Branch("mcpart_trueY_for_hit_for_cluster_vec",&mcpart_trueY_for_hit_for_cluster_vec);
  fTree->Branch("mcpart_trueZ_for_hit_for_cluster_vec",&mcpart_trueZ_for_hit_for_cluster_vec);

  fTree->Branch("mcpart_frac_for_hit_for_cluster_vec",&mcpart_frac_for_hit_for_cluster_vec);
  fTree->Branch("mcpart_matched_energy_for_hit_for_cluster_vec",&mcpart_matched_energy_for_hit_for_cluster_vec);


  // Here we will loop through all the events in that file:
  for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {
    
    // BE SURE TO CLEAR EVERYTHING!
    cluster_x_vec.clear();
    cluster_z_vec.clear();
    cluster_start_wire.clear();
    cluster_end_wire.clear();
    cluster_energy.clear();
    cluster_avg_energy.clear();
    cluster_std_energy.clear();
    cluster_multi_hit_density.clear();
    cluster_IsTrueBeta_vec.clear();
    cluster_IsTrueAlpha_vec.clear();
    cluster_MatchedToMCpart_vec.clear();
    hit_Q_for_cluster_vec.clear();
    hit_sigma_for_cluster_vec.clear();
    hit_GOF_for_cluster_vec.clear();
    hit_mult_for_cluster_vec.clear();
    mcpart_pdg_for_hit_for_cluster_vec.clear();
    mcpart_trueX_for_hit_for_cluster_vec.clear();
    mcpart_trueY_for_hit_for_cluster_vec.clear();
    mcpart_trueZ_for_hit_for_cluster_vec.clear();
    mcpart_primary_for_hit_for_cluster_vec.clear();
    mcpart_frac_for_hit_for_cluster_vec.clear();
    mcpart_matched_energy_for_hit_for_cluster_vec.clear();
    
    // We want to get new "data products" out of the files here
    // currently we are intersted in the truth particles 
    //  .. You can find more data products on the event by doing:
    //       rootstat.py <file_name>.root if uboonecode is setup

    // This is the truth particles
    auto const &mcpart_handle =    
      ev.getValidHandle< std::vector<simb::MCParticle> >("largeant");

    // This is a reconstruction of charge on a single channel 
    auto const &hit_handle =    
      ev.getValidHandle< std::vector<recob::Hit> >("gaushit");

    // This is a grouping of hits
    auto const &cluster_handle =    
      ev.getValidHandle< std::vector<recob::Cluster> >("gaushitproximity");


    //Convert our C++ "pointer" into an object
    // this is now and std::vector<simb::MCParticle> 
    auto all_mcparts(*mcpart_handle);
    auto gaus_hits(*hit_handle);

    std::vector< art::Ptr< recob::Cluster > > clusters;
    art::fill_ptr_vector(clusters, cluster_handle);

    art::FindMany<simb::MCParticle, anab::BackTrackerHitMatchingData> parts_per_hit(hit_handle,ev,"gaushitTruthMatch");     
    art::FindManyP<recob::Hit> hits_per_clust(cluster_handle,ev,"gaushitproximity");     
     
     /**
	the idea would be:
	    cluster_info[c]
	    hits_matched_to_cluster_info[c][h]
	    mcparts_matched_to_hits_matched_to_cluster_info[c][h][m]
      **/

    std::cout << "N clusters : " << clusters.size() << std::endl;
    cluster_x_vec.resize(clusters.size());
    cluster_z_vec.resize(clusters.size());
    cluster_start_wire.resize(clusters.size());
    cluster_end_wire.resize(clusters.size());
    cluster_energy.resize(clusters.size());
    cluster_avg_energy.resize(clusters.size());
    cluster_std_energy.resize(clusters.size());
    cluster_multi_hit_density.resize(clusters.size());
    cluster_IsTrueBeta_vec.resize(clusters.size());
    cluster_IsTrueAlpha_vec.resize(clusters.size());
    cluster_MatchedToMCpart_vec.resize(clusters.size());
    hit_Q_for_cluster_vec.resize(clusters.size());
    hit_sigma_for_cluster_vec.resize(clusters.size());
    hit_GOF_for_cluster_vec.resize(clusters.size());
    hit_mult_for_cluster_vec.resize(clusters.size());
    mcpart_pdg_for_hit_for_cluster_vec.resize(clusters.size());
    mcpart_trueX_for_hit_for_cluster_vec.resize(clusters.size());
    mcpart_trueY_for_hit_for_cluster_vec.resize(clusters.size());
    mcpart_trueZ_for_hit_for_cluster_vec.resize(clusters.size());
    mcpart_primary_for_hit_for_cluster_vec.resize(clusters.size());
    mcpart_frac_for_hit_for_cluster_vec.resize(clusters.size());
    mcpart_matched_energy_for_hit_for_cluster_vec.resize(clusters.size());


     // Let's look at all the clusters 
    for(int cust = 0; cust < clusters.size(); cust++){

      art::Ptr< recob::Cluster > cluster = clusters.at(cust);

      if(cluster->View() != 2) continue; // currently only looking at plane 2

      /**
	 Look at cluster parameters here
	 https://nusoft.fnal.gov/larsoft/doxsvn/html.1.7.1/classrecob_1_1Cluster.html

       **/

      // From Ivan!
      //wire2cm: 0.3
      //time2cm: 0.0548965
      double wire2cm = 0.3;
      double time2cm = 0.0548965;
      double start_wire = cluster->StartWire();
      double end_wire = cluster->EndWire();
      double time = cluster->StartTick();
      double numbhits = cluster->NHits();

      double cluster_z = start_wire*wire2cm;       //Also equal to Cluster_hit_wire_cm
      double cluster_x = (time*time2cm)-44.575 ;   //Also equal to Cluster_hit_time_cm
      // 

      cluster_x_vec[cust] = cluster_x;
      cluster_z_vec[cust] = cluster_z;

      cluster_start_wire[cust] = start_wire;
      cluster_end_wire[cust] = end_wire;
      cluster_energy[cust] = cluster->Integral();
      cluster_avg_energy[cust] = cluster->IntegralAverage();
      cluster_std_energy[cust] = cluster->IntegralStdDev();
      cluster_multi_hit_density[cust] = cluster->MultipleHitDensity();

      bool matched_to_mcpart = false;
      bool matched_to_truth_beta = false;
      bool matched_to_truth_alpha = false;
      auto hits_in_my_cluster = hits_per_clust.at(cluster.key());

      hit_Q_for_cluster_vec[cust].resize(hits_in_my_cluster.size());
      hit_sigma_for_cluster_vec[cust].resize(hits_in_my_cluster.size());
      hit_GOF_for_cluster_vec[cust].resize(hits_in_my_cluster.size());
      hit_mult_for_cluster_vec[cust].resize(hits_in_my_cluster.size());

      mcpart_pdg_for_hit_for_cluster_vec[cust].resize(hits_in_my_cluster.size());
      mcpart_trueX_for_hit_for_cluster_vec[cust].resize(hits_in_my_cluster.size());
      mcpart_trueY_for_hit_for_cluster_vec[cust].resize(hits_in_my_cluster.size());
      mcpart_trueZ_for_hit_for_cluster_vec[cust].resize(hits_in_my_cluster.size());
      mcpart_primary_for_hit_for_cluster_vec[cust].resize(hits_in_my_cluster.size());
      mcpart_frac_for_hit_for_cluster_vec[cust].resize(hits_in_my_cluster.size());
      mcpart_matched_energy_for_hit_for_cluster_vec[cust].resize(hits_in_my_cluster.size());

      // Let's look at the hits matched to clusters
      for(int ht = 0; ht < hits_in_my_cluster.size(); ht++){
	
	auto hit = hits_in_my_cluster.at(ht);

	if(hit->WireID().Plane != 2) std::cout << "You're doing it wrong!" << std::endl;

	for(int ght = 0; ght < gaus_hits.size(); ght++){	
		 
	  auto gaus_hit = gaus_hits.at(ght);
	  
	  if(gaus_hit.RMS() == hit->RMS() &&
	     gaus_hit.Integral() == hit->Integral() && 
	     gaus_hit.StartTick() == hit->StartTick()){
	    
	    /** 
		Look at the hits are matched to that cluster
		https://internal.dunescience.org/doxygen/classrecob_1_1Hit.html
		
	    **/
	    
	    hit_Q_for_cluster_vec[cust][ht] = hit->Integral();
	    hit_sigma_for_cluster_vec[cust][ht] = hit->RMS();
	    hit_GOF_for_cluster_vec[cust][ht] = hit->GoodnessOfFit();
	    hit_mult_for_cluster_vec[cust][ht] = hit->Multiplicity();
	    
	    std::vector<simb::MCParticle const*> parts_in_my_hit;
	    std::vector<anab::BackTrackerHitMatchingData const*> partInfo_in_my_hit;
	    parts_per_hit.get(ght, parts_in_my_hit, partInfo_in_my_hit);
	    	    
	    if(parts_in_my_hit.size() != 0) matched_to_mcpart = true;

	    mcpart_pdg_for_hit_for_cluster_vec[cust][ht].resize(parts_in_my_hit.size());
	    mcpart_trueX_for_hit_for_cluster_vec[cust][ht].resize(parts_in_my_hit.size());
	    mcpart_trueY_for_hit_for_cluster_vec[cust][ht].resize(parts_in_my_hit.size());
	    mcpart_trueZ_for_hit_for_cluster_vec[cust][ht].resize(parts_in_my_hit.size());
	    mcpart_pdg_for_hit_for_cluster_vec[cust][ht].resize(parts_in_my_hit.size());
	    mcpart_primary_for_hit_for_cluster_vec[cust][ht].resize(parts_in_my_hit.size());
	    mcpart_frac_for_hit_for_cluster_vec[cust][ht].resize(parts_in_my_hit.size());
	    mcpart_matched_energy_for_hit_for_cluster_vec[cust][ht].resize(parts_in_my_hit.size());
	   	   
	    //Let's look at the mcparticles matched to hits matched to clusters
	    for(int mcp = 0; mcp < parts_in_my_hit.size(); mcp++){
	      
	      auto mcpart = parts_in_my_hit.at(mcp);
	      auto partInfo = partInfo_in_my_hit.at(mcp);
	      /** 
		  Look at the mcparticles matched to hits that are matched to that cluster
	      **/
	      if(mcpart->PdgCode() != 11)
		std::cout << mcpart->PdgCode() << std::endl;
	      
	      if(mcpart->PdgCode() == 11 && mcpart->Mother() == 0){
		matched_to_truth_beta = true;
	      }
	      
	      if(mcpart->PdgCode() == 1000020040 && mcpart->Mother() == 0){
		matched_to_truth_alpha = true;  
	      }

	      mcpart_pdg_for_hit_for_cluster_vec[cust][ht][mcp] = mcpart->PdgCode();
	      mcpart_trueX_for_hit_for_cluster_vec[cust][ht][mcp] = mcpart->Vx(0);
	      mcpart_trueY_for_hit_for_cluster_vec[cust][ht][mcp] = mcpart->Vy(0);
	      mcpart_trueZ_for_hit_for_cluster_vec[cust][ht][mcp] = mcpart->Vz(0);

	      mcpart_frac_for_hit_for_cluster_vec[cust][ht][mcp] = partInfo->ideNFraction;
	      mcpart_matched_energy_for_hit_for_cluster_vec[cust][ht][mcp] = partInfo->energy;
	      
	      if(mcpart->Mother() == 0){
		mcpart_primary_for_hit_for_cluster_vec[cust][ht][mcp] = 1;
	      }
	      else{
		mcpart_primary_for_hit_for_cluster_vec[cust][ht][mcp] = 0;
	      }

	      
	    }// end iteration over mcparticles
	    break;
	  }//check hits
	  
	}// iterate over gaus hits
	
      }// end iteration over hits

      // here we will all the cluster level information
      cluster_IsTrueBeta_vec[cust] = matched_to_truth_beta;
      cluster_IsTrueAlpha_vec[cust] = matched_to_truth_alpha;
      cluster_MatchedToMCpart_vec[cust] = matched_to_mcpart;

    }// end iteration over clusetrs
    fTree->Fill();

  } // end loop over events 

  //Let's write out our file and tree
  out->cd();  
  fTree->Write();

}// End Program




