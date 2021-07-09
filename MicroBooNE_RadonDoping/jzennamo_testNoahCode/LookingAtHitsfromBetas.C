/// Psuedo code:
/*

  Go through all the truth particles 
  
  Identify the "primary" beta and alpha 
  
  Once identified store their energies
  
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

void LookingAtHitsfromBetas(){

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
  vector<string> filenames {"/pnfs/uboone/scratch/users/jaz8600/BiPo_overlay_BetterDL_2300perEvent/A/v08_00_00_41/44567749_1/BeamOffRun-2018_7_9_0_9_13-0017597-00078_20180713T201835_ext_unbiased_2_20210525T220436_simmxd_detsim_mix_r1a_r1b_r1c_detsim_mix_r1a.root"};
  
  // Here we will create all of our histograms 

  //The range can be checked online, but :
  //    Bi214 beta's cut off at 3.3 MeV
  TH1F* Bi214_betaTrueEnergy = new TH1F("Bi214_betaTrueEnergy","",100,0, 5);  
  // Add new histograms here! 
  TH1F* Bi214_betaRecoEnergy = new TH1F("Bi214_betaRecoEnergy","",100,0, 500);
  TH2F* Bi214_betaTrueVsRecoEnergy = new TH2F("Bi214_betaTrueVsRecoEnergy","",100,0,5,100,0, 500);
  
  TH1F* Po214_Lifetime = new TH1F("Po214_Lifetime","",100,0,1100000);


  //We can also make a file that contains a multidimensional
  //break down for our data, we'll build this for each beta 
  TFile* out = new TFile("outfile_mc_TEST.root","RECREATE");  
  TTree* fTree = new TTree("BiPoTree","ForEachTruthBetaorAlpha");

  double betaTrueEnergy;
  double betaRecoEnergy_plane2;
  double betaCreationTime;
  double betaCreationX;
  double betaCreationY;
  double betaCreationZ;
  double alphaCreationTime;
  double alphaCreationX;
  double alphaCreationY;
  double alphaCreationZ;
  double Po214_lifetime;
  
  fTree->Branch("betaTrueEnergy",&betaTrueEnergy);
  fTree->Branch("betaRecoEnergy_plane2",&betaRecoEnergy_plane2);
  fTree->Branch("betaCreationTime",&betaCreationTime);
  fTree->Branch("betaCreationX",&betaCreationX);
  fTree->Branch("betaCreationY",&betaCreationY);
  fTree->Branch("betaCreationZ",&betaCreationZ);
  fTree->Branch("alphaCreationTime",&alphaCreationTime);
  fTree->Branch("alphaCreationX",&alphaCreationX);
  fTree->Branch("alphaCreationY",&alphaCreationY);
  fTree->Branch("alphaCreationZ",&alphaCreationZ);
  fTree->Branch("Po214_lifetime",&Po214_lifetime);
  

  // Here we will loop through all the events in that file:
  for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {
    
    // We want to get new "data products" out of the files here
    // currently we are intersted in the truth particles 
    //  .. You can find more data products on the event by doing:
    //       rootstat.py <file_name>.root if uboonecode is setup
    auto const &mcpart_handle =    
      ev.getValidHandle< std::vector<simb::MCParticle> >("largeant");

    //Convert our C++ "pointer" into an object
    // this is now and std::vector<simb::MCParticle> 
    auto mcparts(*mcpart_handle);

    art::FindManyP<recob::Hit, anab::BackTrackerHitMatchingData> hit_per_part(mcpart_handle,ev,"gaushitTruthMatch");     
    
    // We can iterate through all the truth particles
    for(int mcp = 0; mcp < mcparts.size(); mcp++){
      
      //We'll want to initialize all our variable here
      betaTrueEnergy = 0;
      betaRecoEnergy_plane2 = 0;
      betaCreationTime = 0;
      betaCreationX = 0;
      betaCreationY = 0;
      betaCreationZ = 0;
      alphaCreationTime = 0;
      alphaCreationX = 0;
      alphaCreationY = 0;
      alphaCreationZ = 0;
      Po214_lifetime = 0;
      
      //reject anything that is not the particles from the decay
      if(mcparts[mcp].Mother() != 0) continue;      

      if(mcparts[mcp].PdgCode() == 11) // check if it is a beta
	{
	  // Here .E(0) is the total starting energy
	  // we are most interested in the kinetic energy 
	  // so we'll subtract the Mass() of the particle
	  // also LArSoft uses GeV so we convert to MeV by *1000
	  
	  betaTrueEnergy = (mcparts[mcp].E(0)-mcparts[mcp].Mass())*1000.;

	  betaCreationTime = mcparts[mcp].T(0);
	  betaCreationX = mcparts[mcp].Position(0).X();
	  betaCreationY = mcparts[mcp].Position(0).Y();
	  betaCreationZ = mcparts[mcp].Position(0).Z();

          for(int mcp1 = 0; mcp1 < mcparts.size(); mcp1++){ //loop to find matching alpha and compute lifetime

            if(mcparts[mcp1].Mother() != 0) continue; // JZ Note: You'll probably want this
            //if(mcparts[mcp].PdgCode() == 11) continue;
            if(mcparts[mcp1].PdgCode() == 1000020040){

              alphaCreationX = mcparts[mcp1].Position(0).X();
	      alphaCreationY = mcparts[mcp1].Position(0).Y();
	      alphaCreationZ = mcparts[mcp1].Position(0).Z();

              if(abs(alphaCreationX-betaCreationX) < 0.00001 && // JZ Note: added ABS here
		 abs(alphaCreationY-betaCreationY) < 0.00001 &&
		 abs(alphaCreationZ-betaCreationZ) < 0.00001){ // JZ Note: this was a typo
                
                alphaCreationTime = mcparts[mcp1].T(0);
                Po214_lifetime = abs(alphaCreationTime-betaCreationTime);
                //if((Po214_lifetime > 0)&&(Po214_lifetime < 1000000)){ //JZ Note: not sure why this is needed
 		Po214_Lifetime->Fill(Po214_lifetime);
		  //}
                break;

	      }
              else{
                continue;
	      }
	    }
            else{
              continue;
	    }
	  }
	    
	  // Let's collect all the hits from these truth particles
	  auto hit_vec = hit_per_part.at(mcp);
	  
	  //this is a vector of all the recob::Hits that are matched
	  // to these truth particles let's iterate through them
	  // and we'll want to add it all up for this particle
	  for(auto hit : hit_vec){
	    
	    if(hit->WireID().Plane == 2){
	      betaRecoEnergy_plane2 += hit->Integral();
	    }//Only study hits on "the good plane"
	    
	  }//End iterate through hits

	  if(betaRecoEnergy_plane2 > 0){
	    Bi214_betaTrueEnergy->Fill(betaTrueEnergy);
	    Bi214_betaRecoEnergy->Fill(betaRecoEnergy_plane2);
	    Bi214_betaTrueVsRecoEnergy->Fill(betaTrueEnergy, betaRecoEnergy_plane2);
	  }
	  else{
	    continue;
	  }
	  //Now we are done with this beta so we 
	  // need to fill the tree
	  fTree->Fill();
	}
      else{continue;}
    } // end loop over truth particles
  } // end loop over events 


  // Now let's make plots!
  // First we make a canvas
  TCanvas* c1 = new TCanvas();
  c1->cd();
  // now we can draw our plots! 
  Bi214_betaTrueEnergy->Draw("");

  TCanvas* c2 = new TCanvas();
  c2->cd();
  // now we can draw our plots! 
  Bi214_betaRecoEnergy->Draw("");
  
  TCanvas* c3 = new TCanvas();
  c3->cd();
  // now we can draw our plots! 
  Bi214_betaTrueVsRecoEnergy->Draw("colz");

  TCanvas* c4 = new TCanvas();
  c4->cd();
  // now we can draw our plots! 
  Po214_Lifetime->Draw("");

  //Let's write out our file and tree
  out->cd();  
  fTree->Write();

}// End Program




