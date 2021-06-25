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

void LookingAtHitsfromBetasPlane1(){

  // create a vector of files we want to process
  std::vector<std::string> filenames;
  
  // read in a file list that we get from a sam-def but remember it 
  // is very long so if we want to run over it all it'll take a while
  // and we'll probably want to break it up on the grid
  ifstream myfile("Reco1FileList.list");
  copy(istream_iterator<string>(myfile),
       istream_iterator<string>(),
       back_inserter(filenames));
  
  //We'll just check the first 10k files for now
  filenames.erase(filenames.begin()+200,filenames.end());
  
  //Here I just hard coding a file for testing, we can adjust this later 
  //vector<string> filenames {"/pnfs/uboone/scratch/users/jaz8600/BiPo_overlay_BetterDL_2300perEvent/A/v08_00_00_41/44567749_1/BeamOffRun-2018_7_9_0_9_13-0017597-00078_20180713T201835_ext_unbiased_2_20210525T220436_simmxd_detsim_mix_r1a_r1b_r1c_detsim_mix_r1a.root"};
  
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
  TH2F* Bi214_PrecisionvsCZ = new TH2F("Bi214_PrecisionvsCZ","",80,0,1100,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsCT = new TH2F("Bi214_PrecisionvsCT","",60,-3000000,3000000,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsReco = new TH2F("Bi214_PrecisionvsReco","",75,0,3.5,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsTrue = new TH2F("Bi214_PrecisionvsTrue","",75,0,3.5,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsXMom = new  TH2F("Bi214_PrecisionvsXMom","",60,-3,3,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsYMom = new TH2F("Bi214_PrecisionvsYMom","",60,-3,3,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsZMom = new TH2F("Bi214_PrecisionvsZMom","",60,-3,3,50,-1.3,2.1);
  TH2F* Bi214_PrecisionvsTMom = new TH2F("Bi214_PrecisionvsTMom","",60,0,3.5,50,-1.3,2.1);
  TH2F* Bi214_PtvsReco = new TH2F("Bi214_PtvsReco","",75,0,3.5,60,0,3.5);
  TH2F* Bi214_CZvsCX = new TH2F("Bi214_CZvsCX","",40,0,280,80,0,1100);
  TH2F* Bi214_CZvsReco = new TH2F("Bi214_CZvsReco","",75,0,3.5,80,0,1100);
  TH2F* Bi214_CXvsReco = new TH2F("Bi214_CXvsReco","",75,0,3.5,40,0,280);

  //We can also make a file that contains a multidimensional
  //break down for our data, we'll build this for each beta 
  TFile* out = new TFile("outfile_mc_TESTCOMPARE.root","RECREATE");  
  TTree* fTree = new TTree("betaTree","ForEachTruthBeta");
  double betaTrueEnergy;
  double betaRecoEnergy_plane1;
  double betaCreationTime;
  double betaCreationX;
  double betaCreationY;
  double betaCreationZ;
  double betaPrecision;
  double betaMomentumX;
  double betaMomentumY;
  double betaMomentumZ;
  double betaMomentumT;
  
  fTree->Branch("betaTrueEnergy",&betaTrueEnergy);
  fTree->Branch("betaRecoEnergy_plane1",&betaRecoEnergy_plane1);
  fTree->Branch("betaCreationTime",&betaCreationTime);
  fTree->Branch("betaCreationX",&betaCreationX);
  fTree->Branch("betaCreationY",&betaCreationY);
  fTree->Branch("betaCreationZ",&betaCreationZ);
  fTree->Branch("betaPrecision",&betaPrecision);
  fTree->Branch("betaMomentumX",&betaMomentumX);
  fTree->Branch("betaMomentumY",&betaMomentumY);
  fTree->Branch("betaMomentumZ",&betaMomentumZ);
  fTree->Branch("betaMomentumT",&betaMomentumT);

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
      betaRecoEnergy_plane1 = 0;
      betaCreationTime = 0;
      betaCreationX = 0;
      betaCreationY = 0;
      betaCreationZ = 0;
      betaMomentumX = 0;
      betaMomentumY = 0;
      betaMomentumZ = 0;
      betaMomentumT = 0;
      
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
	  betaMomentumX = mcparts[mcp].Momentum(0).Px()*1000;
	  betaMomentumY = mcparts[mcp].Momentum(0).Py()*1000;
	  betaMomentumZ = mcparts[mcp].Momentum(0).Pz()*1000;
	  betaMomentumT = mcparts[mcp].Momentum(0).Pt()*1000;

	  // Let's collect all the hits from these truth particles
	  auto hit_vec = hit_per_part.at(mcp);
	  
	  //this is a vector of all the recob::Hits that are matched
	  // to these truth particles let's iterate through them
	  // and we'll want to add it all up for this particle
	  for(auto hit : hit_vec){
	    
	    if(hit->WireID().Plane == 1){
	      betaRecoEnergy_plane1 += hit->Integral();
	    }//Only study hits on "the good plane"
	    
	  }//End iterate through hits

	  betaPrecision = ((betaRecoEnergy_plane1/100)-betaTrueEnergy)/betaTrueEnergy;

	  if(betaRecoEnergy_plane1 > 0 && betaPrecision < 5){
	    Bi214_betaTrueEnergy->Fill(betaTrueEnergy);
	    Bi214_betaRecoEnergy->Fill(betaRecoEnergy_plane1/100.0);
	    Bi214_betaTrueVsRecoEnergy->Fill(betaTrueEnergy, betaRecoEnergy_plane1/100.0);
	    Bi214_PrecisionvsCX->Fill(betaCreationX,betaPrecision);
	    Bi214_PrecisionvsCY->Fill(betaCreationY,betaPrecision);
	    Bi214_PrecisionvsCZ->Fill(betaCreationZ,betaPrecision);
	    Bi214_PrecisionvsCT->Fill(betaCreationTime,betaPrecision);
	    Bi214_PrecisionvsReco->Fill(betaRecoEnergy_plane1/100.0,betaPrecision);
	    Bi214_PrecisionvsTrue->Fill(betaTrueEnergy,betaPrecision);
	    Bi214_PrecisionvsXMom->Fill(betaMomentumX,betaPrecision);
	    Bi214_PrecisionvsYMom->Fill(betaMomentumY,betaPrecision);
	    Bi214_PrecisionvsZMom->Fill(betaMomentumZ,betaPrecision);
	    Bi214_PrecisionvsTMom->Fill(betaMomentumT,betaPrecision);
	    Bi214_PtvsReco->Fill(betaRecoEnergy_plane1/100,betaMomentumT);
	    Bi214_CZvsCX->Fill(betaCreationX,betaCreationZ);
	    Bi214_CZvsReco->Fill(betaRecoEnergy_plane1/100,betaCreationZ);
	    Bi214_CXvsReco->Fill(betaRecoEnergy_plane1/100,betaCreationX);
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
  Bi214_PrecisionvsReco->GetYaxis()->SetTitle("Precision");
  Bi214_PrecisionvsReco->GetXaxis()->SetTitle("Reco Energy (~MeV)");
  Bi214_PrecisionvsReco->SetTitle("Plane 1: Precision vs Reco");
  Bi214_PrecisionvsReco->Draw("colz");
  Bi214_PrecisionvsReco->SaveAs("Plane1_PrecisionvsReco.root");
  /*
  TCanvas* c2 = new TCanvas();
  c2->cd();
  // now we can draw our plots!
  Bi214_betaRecoEnergy->GetYaxis()->SetTitle("Number of Betas");
  Bi214_betaRecoEnergy->GetXaxis()->SetTitle("Reconstructed Energy (~MeV)");
  Bi214_betaRecoEnergy->SetTitle("Plane 1: Reconstructed Energy Beta Spectrum");
  Bi214_betaRecoEnergy->Draw("");
  
  TCanvas* c3 = new TCanvas();
  c3->cd();
  // now we can draw our plots! 
  Bi214_betaTrueVsRecoEnergy->GetXaxis()->SetTitle("True Energy (MeV)");
  Bi214_betaTrueVsRecoEnergy->GetYaxis()->SetTitle("Reconstructed Energy (~MeV)");
  Bi214_betaTrueVsRecoEnergy->SetTitle("Plane 1: Reconstructed vs. True Beta Energy");
  Bi214_betaTrueVsRecoEnergy->Draw("colz");

  TCanvas* c4 = new TCanvas();
  c4->cd();
  // now we can draw our plots!
  Bi214_betaFailedRecoTime->GetYaxis()->SetTitle("Number of Betas");
  Bi214_betaFailedRecoTime->GetXaxis()->SetTitle("Reconstructed Time (microsec)");
  Bi214_betaFailedRecoTime->SetTitle("Plane 1: Reconstructed Time of Betas");
  Bi214_betaFailedRecoTime->Draw("");

  TCanvas* c5 = new TCanvas();
  c5->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsCX->GetXaxis()->SetTitle("Start X Position (cm)");
  Bi214_PrecisionvsCX->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsCX->SetTitle("Plane 1: Precision vs. X Creation Vertex");
  Bi214_PrecisionvsCX->Draw("colz");

  TCanvas* c6 = new TCanvas();
  c6->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsCY->GetXaxis()->SetTitle("Start Y Position (cm)");
  Bi214_PrecisionvsCY->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsCY->SetTitle("Plane 1: Precision vs. Y Creation Vertex");
  Bi214_PrecisionvsCY->Draw("colz");

  TCanvas* c7 = new TCanvas();
  c7->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsCZ->GetXaxis()->SetTitle("Start Z Position (cm)");
  Bi214_PrecisionvsCZ->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsCZ->SetTitle("Plane 1: Precision vs. Z Creation Vertex");
  Bi214_PrecisionvsCZ->Draw("colz");

  TCanvas* c8 = new TCanvas();
  c8->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsCT->GetXaxis()->SetTitle("Reconstructed Time (microsecs)");
  Bi214_PrecisionvsCT->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsCT->SetTitle("Plane 1: Precision vs. Reconstructed Time");
  Bi214_PrecisionvsCT->Draw("colz");

  TCanvas* c9 = new TCanvas();
  c9->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsReco->GetXaxis()->SetTitle("Reconstructed Energy (~MeV)");
  Bi214_PrecisionvsReco->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsReco->SetTitle("Plane 1: Precision vs. Reconstructed Energy");
  Bi214_PrecisionvsReco->Draw("colz");

  TCanvas* c10 = new TCanvas();
  c10->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsTrue->GetXaxis()->SetTitle("True Energy (MeV)");
  Bi214_PrecisionvsTrue->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsTrue->SetTitle("Plane 1: Precision vs. True Energy");
  Bi214_PrecisionvsTrue->Draw("colz");

  TCanvas* c11 = new TCanvas();
  c11->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsXMom->GetXaxis()->SetTitle("Start X Momentum (MeV)");
  Bi214_PrecisionvsXMom->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsXMom->SetTitle("Plane 1: Precision vs. X Momentum");
  Bi214_PrecisionvsXMom->Draw("colz");

  TCanvas* c12 = new TCanvas();
  c12->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsYMom->GetXaxis()->SetTitle("Start Y Momentum (MeV)");
  Bi214_PrecisionvsYMom->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsYMom->SetTitle("Plane 1: Precision vs. Y Momentum");
  Bi214_PrecisionvsYMom->Draw("colz");

  TCanvas* c13 = new TCanvas();
  c13->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsZMom->GetXaxis()->SetTitle("Start Z Momentum (MeV)");
  Bi214_PrecisionvsZMom->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsZMom->SetTitle("Plane 1: Precision vs. Z Momentum");
  Bi214_PrecisionvsZMom->Draw("colz");

  TCanvas* c14 = new TCanvas();
  c14->cd();
  // now we can draw our plots! 
  Bi214_PrecisionvsTMom->GetXaxis()->SetTitle("Start Transverse Momentum (MeV)");
  Bi214_PrecisionvsTMom->GetYaxis()->SetTitle("Precision (Reco - True/True)");
  Bi214_PrecisionvsTMom->SetTitle("Plane 1: Precision vs. Transverse Momentum");
  Bi214_PrecisionvsTMom->Draw("colz");

  TCanvas* c15 = new TCanvas();
  c15->cd();
  // now we can draw our plots! 
  Bi214_PtvsReco->GetXaxis()->SetTitle("Reconstructed Energy(~MeV)");
  Bi214_PtvsReco->GetYaxis()->SetTitle("Transverse Momentum (MeV)");
  Bi214_PtvsReco->SetTitle("Plane 1: Transverse Momentum vs. Reconstructed Energy");
  Bi214_PtvsReco->Draw("colz");

  TCanvas* c16 = new TCanvas();
  c16->cd();
  // now we can draw our plots! 
  Bi214_CZvsCX->GetXaxis()->SetTitle("Creation X (cm)");
  Bi214_CZvsCX->GetYaxis()->SetTitle("Creation Z (cm)");
  Bi214_CZvsCX->SetTitle("Plane 1: Creation Z vs. Creation X");
  Bi214_CZvsCX->Draw("colz");

  TCanvas* c17 = new TCanvas();
  c17->cd();
  // now we can draw our plots! 
  Bi214_CZvsReco->GetXaxis()->SetTitle("Reconstructed Energy (~MeV)");
  Bi214_CZvsReco->GetYaxis()->SetTitle("Creation Z (cm)");
  Bi214_CZvsReco->SetTitle("Plane 1: Creation Z vs. Reconstructed Energy");
  Bi214_CZvsReco->Draw("colz");

  TCanvas* c18 = new TCanvas();
  c18->cd();
  // now we can draw our plots! 
  Bi214_CXvsReco->GetXaxis()->SetTitle("Reconstructed Energy (~MeV)");
  Bi214_CXvsReco->GetYaxis()->SetTitle("Creation X (cm)");
  Bi214_CXvsReco->SetTitle("Plane 1: Creation X vs. Reconstructed Energy");
  Bi214_CXvsReco->Draw("colz");
  */
  //Let's write out our file and tree
  out->cd();  
  fTree->Write();

}// End Program




