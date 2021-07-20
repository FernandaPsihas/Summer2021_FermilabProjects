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

void CheckingHits(){

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
  vector<string> filenames {"/pnfs/uboone/scratch/users/ilepetic/gamma3dgrid/v08_00_00_45/ilepetic_gamma3d_newoverlays/21080126_2/output.root"};
  //  vector<string> filenames {"/pnfs/uboone/scratch/users/jaz8600/BiPo_overlay_BetterDL_2300perEvent/A/v08_00_00_41/44567749_1/BeamOffRun-2018_7_9_0_9_13-0017597-00078_20180713T201835_ext_unbiased_2_20210525T220436_simmxd_detsim_mix_r1a_r1b_r1c_detsim_mix_r1a.root"};
  
  // Here we will create all of our histograms 

  //The range can be checked online, but :
  //    Bi214 beta's cut off at 3.3 MeV
  TH1F* Bi214_betaTrueEnergy = new TH1F("Bi214_betaTrueEnergy","",100,0, 5);  
  // Add new histograms here! 
  TH1F* Bi214_betaRecoEnergy = new TH1F("Bi214_betaRecoEnergy","",100,0, 500);
  TH2F* Bi214_betaTrueVsRecoEnergy = new TH2F("Bi214_betaTrueVsRecoEnergy","",100,0,5,100,0, 500);
  
  TH1F* Bi214_betaFailedRecoTime = new TH1F("Bi214_betaFailedRecoTime","",1000,-5000000, 5000000);


  //We can also make a file that contains a multidimensional
  //break down for our data, we'll build this for each beta 
  TFile* out = new TFile("outfile_mc_TEST.root","RECREATE");  
  TTree* fTree = new TTree("betaTree","ForEachTruthBeta");
  double betaTrueEnergy;
  double betaRecoEnergy_plane2;
  double betaMaxADCHit_plane2;
  double betaCreationTime;
  double betaCreationX;
  double betaCreationY;
  double betaCreationZ;
  
  fTree->Branch("betaTrueEnergy",&betaTrueEnergy);
  fTree->Branch("betaRecoEnergy_plane2",&betaRecoEnergy_plane2);
  fTree->Branch("betaMaxADCHit_plane2",&betaMaxADCHit_plane2);
  fTree->Branch("betaCreationTime",&betaCreationTime);
  fTree->Branch("betaCreationX",&betaCreationX);
  fTree->Branch("betaCreationY",&betaCreationY);
  fTree->Branch("betaCreationZ",&betaCreationZ);

  // Here we will loop through all the events in that file:
  for (gallery::Event ev(filenames) ; !ev.atEnd(); ev.next()) {

    auto const &hit_handle1 =    
      ev.getValidHandle< std::vector<recob::Hit> >("cosmicfilter");

    auto const &hit_handle2 =    
      ev.getValidHandle< std::vector<recob::Hit> >("gaushit");


    std::vector< art::Ptr< recob::Hit > > hits1;
    art::fill_ptr_vector(hits1, hit_handle1);

    std::vector< art::Ptr< recob::Hit > > hits2;
    art::fill_ptr_vector(hits2, hit_handle2);

    for(int h1 = 0; h1 < hits1.size(); h1++){
      auto hit1 = hits1.at(h1);
      
      for(int h2 = 0; h2 < hits2.size(); h2++){ 
	auto hit2 = hits2.at(h2);

	if(hit1->RMS() == hit2->RMS() &&
	   hit1->Integral() == hit2->Integral() && 
	   hit1->StartTick() == hit2->StartTick()){
	  std::cout << "key1 : " << hit1.key() << ", key2: " << hit2.key() << std::endl;
	}

    }
  }
  }
}// End Program




