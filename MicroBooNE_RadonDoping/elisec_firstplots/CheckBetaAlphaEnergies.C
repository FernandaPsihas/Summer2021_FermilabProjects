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
#include "lardataobj/AnalysisBase/BackTrackerMatchingData.h"

//This way you can be lazy
using namespace art;
using namespace std;

void CheckBetaAlphaEnergies(){
  
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
  TFile* out = new TFile("outfile_mc_TEST.root","RECREATE");  
  TTree* fTree = new TTree("hit_v_track","HitPropertiesTrackProperties");
  
  //The range can be checked online, but :
  //    Bi214 beta's cut off at 3.3 MeV and
  //    Po214 alpha's are monoenergetic at ~7.7 MeV
  TH1F* Bi214_betaEnergy = new TH1F("Bi214_betaEnergy",";^{214}Bi #beta Energy [MeV]; Events",100,0, 5);
  TH1F* Po214_alphaEnergy = new TH1F("Po214_alphaEnergy",";^{214}Po #alpha Energy [MeV]; Events",100,6, 8);
  // Add new histograms here! 


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
    
    // We can iterate through all the truth particles
    for(int mcp = 0; mcp < mcparts.size(); mcp++){
      
      //reject anything that is not the particles from the decay
      if(mcparts[mcp].Mother() != 0) continue;      

      if(mcparts[mcp].PdgCode() == 11) // check if it is a beta
	{
	  // Here .E(0) is the total starting energy
	  // we are most interested in the kinetic energy 
	  // so we'll subtract the Mass() of the particle
	  // also LArSoft uses GeV so we convert to MeV by *1000
 	  Bi214_betaEnergy->Fill((mcparts[mcp].E(0)-mcparts[mcp].Mass())*1000.);
	} // end beta check
      else if(mcparts[mcp].PdgCode() == 1000020040)
	{
	  Po214_alphaEnergy->Fill((mcparts[mcp].E(0)-mcparts[mcp].Mass())*1000.);
	} // end alpha check
      else
	{continue;}//throw away everything else
    } // end loop over truth particles
  } // end loop over events 

  // Now let's make plots!
  // First we make a canvas
  TCanvas* c1 = new TCanvas();
  c1->cd();
  // now we can draw our plots!
  Bi214_betaEnergy->Fit("pol6");
  Bi214_betaEnergy->Draw();
  
  TCanvas* c2 = new TCanvas();
  c2->cd();
  Po214_alphaEnergy->Draw();
  
}// End Program


