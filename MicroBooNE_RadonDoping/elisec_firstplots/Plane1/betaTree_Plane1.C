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
  std::vector<TH1D*> EnergyResolutionVsBetaY;
  std::vector<TH1D*> EnergyResolutionVsBetaX;
  std::vector<TH1D*> EnergyResolutionVsBetaReco;
  std::vector<TH1D*> EnergyResolutionVsBetaTrue;
  //We'll want 10 bins in Z
  EnergyResolutionVsBetaZ.resize(10);
  EnergyResolutionVsBetaX.resize(10);
  EnergyResolutionVsBetaY.resize(10);
  EnergyResolutionVsBetaReco.resize(10);
  EnergyResolutionVsBetaTrue.resize(10);
  //Our 10 bins will need 11 bin edges
  double xBins[11] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150};
  double yBins[11] = {-120, -96, -72, -48, -24, 0, 24, 48, 72, 96, 120};
  double zBins[11] = {0, 105, 210, 315, 420, 525, 630, 735, 840, 945, 1050};
  double recoBins[11] = {0.15, 0.235, 0.32, 0.405, 0.49, 0.575, 0.66, 0.745, 0.83, .915, 1};
  double trueBins[11] = {0, 0.12, 0.36, 0.48, 0.60, 0.72, 0.84, 0.96, 1.08, 1.2};
  
  for(int i = 0;i < EnergyResolutionVsBetaZ.size(); i++){
    //initialize each of these plots
    EnergyResolutionVsBetaZ[i] = new TH1D(Form("EnergyResolution_in_BetaZ_bin_%d", i), ";Energy Resolution;Counts",100, -2, 2);
  }

  for(int i = 0;i < EnergyResolutionVsBetaY.size(); i++){
    //initialize each of these plots
    EnergyResolutionVsBetaY[i] = new TH1D(Form("EnergyResolution_in_BetaY_bin_%d", i), ";Energy Resolution;Counts",100, -2, 2);
  }

  for(int i = 0;i < EnergyResolutionVsBetaX.size(); i++){
    //initialize each of these plots
    EnergyResolutionVsBetaX[i] = new TH1D(Form("EnergyResolution_in_BetaX_bin_%d", i), ";Energy Resolution;Counts",100, -2, 2);
  }
  
  for(int i = 0;i < EnergyResolutionVsBetaReco.size(); i++){
    //initialize each of these plots
    EnergyResolutionVsBetaReco[i] = new TH1D(Form("EnergyResolution_in_BetaReco_bin_%d", i), ";Energy Resolution;Counts",100, -2, 2);
  }
  
  for(int i = 0;i < EnergyResolutionVsBetaTrue.size(); i++){
    //initialize each of these plots
    EnergyResolutionVsBetaTrue[i] = new TH1D(Form("EnergyResolution_in_BetaTrue_bin_%d", i), ";Energy Resolution;Counts",100, -2, 2);
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

      double energyResolution = ((betaRecoEnergy_plane1/100)-betaTrueEnergy)/betaTrueEnergy;

      for(int zbin = 0; zbin < 10; zbin++){
      
	if(betaCreationZ > zBins[zbin] && betaCreationZ <= zBins[zbin+1]){
	  EnergyResolutionVsBetaZ[zbin]->Fill(energyResolution);

	}
	
      }
   
      for(int xbin = 0; xbin < 10; xbin++){
      
	if(betaCreationX > xBins[xbin] && betaCreationX <= xBins[xbin+1]){
	  EnergyResolutionVsBetaX[xbin]->Fill(energyResolution);

	}
	
      }

      for(int ybin = 0; ybin < 10; ybin++){
      
	if(betaCreationY > yBins[ybin] && betaCreationY <= yBins[ybin+1]){
	  EnergyResolutionVsBetaY[ybin]->Fill(energyResolution);

	}
	
      }
      
      for(int recobin = 0; recobin < 10; recobin++){
      
	if(betaRecoEnergy_plane1/100 > recoBins[recobin] && betaRecoEnergy_plane1/100 <= recoBins[recobin+1]){
	  EnergyResolutionVsBetaReco[recobin]->Fill(energyResolution);

	}
	
      }

      for(int truebin = 0; truebin < 10; truebin++){
      
	if(betaTrueEnergy > trueBins[truebin] && betaTrueEnergy <= trueBins[truebin+1]){
	  EnergyResolutionVsBetaTrue[truebin]->Fill(energyResolution);

	}
	
      }
      
   }

   std::vector<TCanvas*> c; c.resize(50);

   for(int i = 0; i < 10; i++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaX[i]->Draw();
   }

   for(int i = 10, j = 0; i < 20 && j < 10; i++, j++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaY[j]->Draw();
   }

   for(int i = 20, j = 0; i < 30 && j < 10; i++, j++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaZ[j]->Draw();
   }

   for(int i = 30, j = 0; i < 40 && j < 10; i++, j++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaReco[j]->Draw();
   }

   for(int i = 40, j = 0; i < 50 && j < 10; i++, j++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaTrue[j]->Draw();
   }

}

