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

void LookingAtHitsfromBetasPlane0(){

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
  TH1F* Bi214_betaRecoEnergy = new TH1F("Bi214_betaRecoEnergy","",100,0, 5);
  TH2F* Bi214_betaTrueVsRecoEnergy = new TH2F("Bi214_betaTrueVsRecoEnergy","",100,0,5,100,0, 5);
  
  TH1F* Bi214_betaFailedRecoTime = new TH1F("Bi214_betaFailedRecoTime","",1000,-5000000, 5000000);
  TH2F* Bi214_PrecisionvsCX = new TH2F("Bi214_PrecisionvsCX","",40,0,280,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsCY = new TH2F("Bi214_PrecisionvsCY","",60,-120,120,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsCZ = new TH2F("Bi214_PrecisionvsCZ","",40,0,280,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsCT = new TH2F("Bi214_PrecisionvsCT","",60,-3000000,3000000,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsReco = new TH2F("Bi214_PrecisionvsReco","",75,0,3.5,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsTrue = new TH2F("Bi214_PrecisionvsTrue","",75,0,3.5,50,-1.3,2.1);

  //We can also make a file that contains a multidimensional
  //break down for our data, we'll build this for each beta 
  TFile* out = new TFile("outfile_mc_TESTCOMPARE.root","RECREATE");  
  TTree* fTree = new TTree("betaTree","ForEachTruthBeta");
  double betaTrueEnergy;
  double betaRecoEnergy_plane0;
  double betaCreationTime;
  double betaCreationX;
  double betaCreationY;
  double betaCreationZ;
  double betaPrecision;
  
  fTree->Branch("betaTrueEnergy",&betaTrueEnergy);
  fTree->Branch("betaRecoEnergy_plane0",&betaRecoEnergy_plane0);
  fTree->Branch("betaCreationTime",&betaCreationTime);
  fTree->Branch("betaCreationX",&betaCreationX);
  fTree->Branch("betaCreationY",&betaCreationY);
  fTree->Branch("betaCreationZ",&betaCreationZ);
  fTree->Branch("betaPrecision",&betaPrecision);

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
      betaRecoEnergy_plane0 = 0;
      betaCreationTime = 0;
      betaCreationX = 0;
      betaCreationY = 0;
      betaCreationZ = 0;
      
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
	    
	  // Let's collect all the hits from these truth particles
	  auto hit_vec = hit_per_part.at(mcp);
	  
	  //this is a vector of all the recob::Hits that are matched
	  // to these truth particles let's iterate through them
	  // and we'll want to add it all up for this particle
	  for(auto hit : hit_vec){
	    
	    if(hit->WireID().Plane == 0){
	      betaRecoEnergy_plane0 += hit->Integral();
	    }//Let's look at plane 0
	    
	  }//End iterate through hits

	  betaPrecision = ((betaRecoEnergy_plane0/100.0)-betaTrueEnergy)/betaTrueEnergy;

	  if(betaRecoEnergy_plane0 > 0 && betaPrecision < 5){
	    Bi214_betaTrueEnergy->Fill(betaTrueEnergy);
	    Bi214_betaRecoEnergy->Fill(betaRecoEnergy_plane0/100.0);
	    Bi214_betaTrueVsRecoEnergy->Fill(betaTrueEnergy, betaRecoEnergy_plane0/100.0);
	    Bi214_PrecisionvsCX->Fill(betaCreationX,betaPrecision);
	    Bi214_PrecisionvsCY->Fill(betaCreationY,betaPrecision);
	    Bi214_PrecisionvsCZ->Fill(betaCreationZ,betaPrecision);
	    Bi214_PrecisionvsCT->Fill(betaCreationTime,betaPrecision);
	    Bi214_PrecisionvsReco->Fill(betaRecoEnergy_plane0/100.0,betaPrecision);
	    Bi214_PrecisionvsTrue->Fill(betaTrueEnergy,betaPrecision);
	  }
	  else{
	    Bi214_betaFailedRecoTime->Fill(betaCreationTime);
	  }
	  //Now we are done with this beta so we 
	  // need to fill the tree
	  fTree->Fill();
	} // end beta check
      else
	{continue;}//throw away everything else
    } // end loop over truth particles
  } // end loop over events 

  gStyle->SetOptStat(0);
  // Now let's make plots!
  // First we make a canvas
  TCanvas* c1 = new TCanvas();
  c1->cd();
  // now we can draw our plots!
  Bi214_betaTrueEnergy->GetYaxis()->SetTitle("Number of Betas");
  Bi214_betaTrueEnergy->GetXaxis()->SetTitle("True Beta Energy (MeV)");
  Bi214_betaTrueEnergy->SetTitle("Plane 0: True Energy Beta Spectrum");
  Bi214_betaTrueEnergy->Draw("");

  TCanvas* c2 = new TCanvas();
  c2->cd();
  // now we can draw our plots!
  Bi214_betaRecoEnergy->GetYaxis()->SetTitle("Number of Betas");
  Bi214_betaRecoEnergy->GetXaxis()->SetTitle("Reconstructed Energy (~MeV)");
  Bi214_betaRecoEnergy->SetTitle("Plane:0 Reconstructed Energy Beta Spectrum");
  Bi214_betaRecoEnergy->Draw("");
  
  TCanvas* c3 = new TCanvas();
  c3->cd();
  // now we can draw our plots! 
  Bi214_betaTrueVsRecoEnergy->GetXaxis()->SetTitle("True Energy (MeV)");
  Bi214_betaTrueVsRecoEnergy->GetYaxis()->SetTitle("Reconstructed Energy (~MeV)");
  Bi214_betaTrueVsRecoEnergy->SetTitle("Plane 0: Reconstructed vs. True Beta Energy");
  Bi214_betaTrueVsRecoEnergy->Draw("colz");

  TCanvas* c4 = new TCanvas();
  c4->cd();
  // now we can draw our plots!
  Bi214_betaFailedRecoTime->GetYaxis()->SetTitle("Number of Betas");
  Bi214_betaFailedRecoTime->GetXaxis()->SetTitle("Reconstructed Time (unit?)");
  Bi214_betaFailedRecoTime->SetTitle("Plane 0: Reconstructed Time of Betas");
  Bi214_betaFailedRecoTime->Draw("");

  TCanvas* c5 = new TCanvas();
  c5->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsCX->GetXaxis()->SetTitle("Start X Position (unit?)");
  Bi214_PrecisionvsCX->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsCX->SetTitle("Plane 0: Precision vs. X Creation Vertex");
  Bi214_PrecisionvsCX->Draw("colz");

  TCanvas* c6 = new TCanvas();
  c6->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsCY->GetXaxis()->SetTitle("Start Y Position (unit?)");
  Bi214_PrecisionvsCY->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsCY->SetTitle("Plane 0: Precision vs. Y Creation Vertex");
  Bi214_PrecisionvsCY->Draw("colz");

  TCanvas* c7 = new TCanvas();
  c7->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsCZ->GetXaxis()->SetTitle("Start Z Position (unit?)");
  Bi214_PrecisionvsCZ->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsCZ->SetTitle("Plane 0: Precision vs. Z Creation Vertex");
  Bi214_PrecisionvsCZ->Draw("colz");

  TCanvas* c8 = new TCanvas();
  c8->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsCT->GetXaxis()->SetTitle("Reconstructed Time (unit?)");
  Bi214_PrecisionvsCT->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsCT->SetTitle("Plane 0: Precision vs. Reconstructed Time");
  Bi214_PrecisionvsCT->Draw("colz");

  TCanvas* c9 = new TCanvas();
  c9->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsReco->GetXaxis()->SetTitle("Reconstructed Energy (~MeV)");
  Bi214_PrecisionvsReco->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsReco->SetTitle("Plane 0: Precision vs. Reconstructed Energy");
  Bi214_PrecisionvsReco->Draw("colz");

  TCanvas* c10 = new TCanvas();
  c10->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsTrue->GetXaxis()->SetTitle("True Energy (MeV)");
  Bi214_PrecisionvsTrue->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsTrue->SetTitle("Plane 0: Precision vs. True Energy");
  Bi214_PrecisionvsTrue->Draw("colz");

  //Let's write out our file and tree
  out->cd();  
  fTree->Write();

}// End Program




