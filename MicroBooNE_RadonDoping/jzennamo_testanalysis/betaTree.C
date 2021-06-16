#define betaTree_cxx
#include "betaTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void betaTree::Loop()
{
//   In a ROOT session, you can do:
//      root> .L betaTree.C
//      root> betaTree t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries

//Here we can create all of our plots
  // Let's start of by being fancy ;) 
  // We'll make a vector of histograms
  std::vector<TH1D*> EnergyResolutionVsBetaZ;
  // We'll want 10 bins in Z
  EnergyResolutionVsBetaZ.resize(10);
  // Our 10 bins will need 11 bin edges
  double zBins[11] = {0, 103.6, 207.2, 310.8, 414.4, 518, 621.6, 725.2, 828.8, 932.4, 1036};
  
  for(int i = 0; i < EnergyResolutionVsBetaZ.size(); i++){
    //We'll need to initialize each of these plots
    EnergyResolutionVsBetaZ[i] = new TH1D(Form("EnergyResolution_in_BetaZ_bin_%d", i), ";Energy Resolution;Counts",100, 0, 200);
  }

  //Ignore this it is what root needs
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   // Here we then loop through all the events in the TTree that we made
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      if(betaRecoEnergy_plane2 == 0) continue;
      
      double energyResolution = (betaRecoEnergy_plane2-betaTrueEnergy)/betaTrueEnergy;

      for(int zbin = 0; zbin < 10; zbin++){
	
	if(betaCreationZ > zBins[zbin] && betaCreationZ <= zBins[zbin+1]){
	  EnergyResolutionVsBetaZ[zbin]->Fill(energyResolution);
	}

      }
               
   }
   
   std::vector<TCanvas*> c; c.resize(10);

   for(int i = 0; i < 10; i++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaZ[i]->Draw();
   }

}
