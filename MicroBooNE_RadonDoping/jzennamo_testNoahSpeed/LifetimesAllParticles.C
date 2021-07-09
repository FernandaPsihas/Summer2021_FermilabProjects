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
#include "TGraph.h"
#include "TF1.h"

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
TGraph *PoLifetime;
TGraph *PoPopulation;
double FittedFunction(double * xx, double *par)
{
  return par[0]*PoLifetime->Eval(xx[0]);
}

double FittedExponential(double *t, double *T){
  return 77973*exp(-T[0]*t[0]);
}

void LifetimesAllParticles(){

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

  TH1F* Candidate_Lifetimes = new TH1F("Candidate Lifetimes","", 1000,0,1000000);
  //TH1F* Po214_Lifetime = new TH1F("Po214_Lifetime","",1000,0,1000000);
  //TH1F* Rejected_Lifetimes = new TH1F("Rejected Lifetimes","",1000,0,1000000);

  //We can also make a file that contains a multidimensional
  //break down for our data, we'll build this for each beta 
  TFile* out = new TFile("outfile_mc_TEST1.root","RECREATE");  
  TTree* cTree = new TTree("Candidate Tree","ForAllPairsofParticles");

  int particleind;
  double particleX;
  double particleY;
  double particleZ;
  double particleTime;
  int particle1ind;
  double particle1X;
  double particle1Y;
  double particle1Z;
  double particle1Time;
  std::vector<int> candidatePair;
  double candidateLifetime;
  
  cTree->Branch("particleind",&particleind);
  cTree->Branch("particleX",&particleX);
  cTree->Branch("particleY",&particleY);
  cTree->Branch("particleZ",&particleZ);
  cTree->Branch("particleTime",&particleTime);
  cTree->Branch("particle1ind",&particle1ind);
  cTree->Branch("particle1X",&particle1X);
  cTree->Branch("particle1Y",&particle1Y);
  cTree->Branch("particle1Z",&particle1Z);
  cTree->Branch("particle1Time",&particle1Time);
  cTree->Branch("Candidate Pairs",&candidatePair);
  cTree->Branch("Candidate Lifetimes",&candidateLifetime);
  

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

      //To cut the dataset down to a calculable size, we will skip over certain particle types & energies
      if(mcparts[mcp].PdgCode() == 22) continue;  //ignore photons
      if((mcparts[mcp].E(0)-mcparts[mcp].Mass())*1000. < 0.1) continue;  //ignore all particles < 100 keV
      
      //We'll want to initialize all our variable here
      particleind = mcp;
      particleX = mcparts[mcp].Position(0).X();
      particleY = mcparts[mcp].Position(0).Y();
      particleZ = mcparts[mcp].Position(0).Z();
      particleTime = mcparts[mcp].T(0);
      printf("%d\n",mcp);

      for(int mcp1 = mcp+1; mcp1 < mcparts.size(); mcp1++){

        //implement the same process as before
        if(mcparts[mcp1].PdgCode() == 22) continue;  //ignore photons
        if((mcparts[mcp1].E(0)-mcparts[mcp1].Mass())*1000. < 0.1) continue;  //ignore all particles < 100 keV

        particle1ind = mcp1;
        particle1X = mcparts[mcp1].Position(0).X();
        particle1Y = mcparts[mcp1].Position(0).Y();
        particle1Z = mcparts[mcp1].Position(0).Z();
        particle1Time = mcparts[mcp1].T(0); 

        if((abs(particleY-particle1Y)<0.01)&&(abs(particleZ-particle1Z)<0.01)){   //candidate pairs have same Y and Z, but only BiPo pairs have same X

          candidatePair.push_back(particleind);
          candidatePair.push_back(particle1ind);
          candidateLifetime = abs(particleTime-particle1Time);
          Candidate_Lifetimes->Fill(candidateLifetime);
          cTree->Fill();
          //printf("%d\n", mcp1);
          break;
	}
      }
    }
  }

  TCanvas* c1 = new TCanvas();
  c1->cd();
  Candidate_Lifetimes->Draw("");


  out->cd();
  cTree->Write();

}
  
