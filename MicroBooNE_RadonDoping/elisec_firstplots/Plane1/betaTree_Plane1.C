#define betaTree_Plane1_cxx
#include "betaTree_Plane1.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void betaTree_Plane1::Loop()
{
//   In a ROOT session, you can do:
//      root> .L betaTree_Plane1.C
//      root> betaTree_Plane1 t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

//Here we can create all of our plots
  //Vector of histogram
  std::vector<TH1D*> EnergyResolutionVsBetaZ;
  //We'll want 10 bins in Z
  EnergyResolutionVsBetaZ.resize(10);
  //Our 10 bins will need 11 bin edges
  double zBins[11] = {0, 103.6, 207.2, 310.8, 414.4, 518, 621.6, 725.2, 828.8, 932.4, 1036};
  
  for(int i = 0;i < EnergyResolutionVsBetaZ.size(); i++){
    //initialize each of these plots
    EnergyResolutionVsBetaZ[i] = TH1D(Form("EnergyResolution_in_BetaZ_bin_%d", i), ";Energy Resolution;Counts",100, 0, 200);
  }

  //Stuff for root
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   //Loop through all events in TTree
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      if(betaRecoEnergy_plane1 == 0) continue;

      double energyResolution = (betaRecoEnergy_plane1-betaTrueEnergy)/betaTrueEnergy;

      for(int zbin = 0; zbin < 10; zbin++){
      
	if(betaCreationZ > zBins[zbin] && betaCreationZ <= zBins[zbin+1]){
	  EnergyResolutionVsBetaZ[zbin]->Fill(energyResolution);

	}
	
      }
      
   }

   std::vector<Tcanvas*> c; c.resize(10);

   for(int i = 0; i < 10; i++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaZ[i]->Draw();
   }
}

