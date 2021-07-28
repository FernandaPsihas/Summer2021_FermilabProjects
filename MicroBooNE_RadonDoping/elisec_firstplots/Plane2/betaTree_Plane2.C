#define betaTree_Plane2_cxx
#include "betaTree_Plane2.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>

double ErrorProp(double A, double B, double SigA, double SigB){
  double f;
  double AErr;
  double BErr;
  double PartErrSq;
  double Error;
  
  f = TMath::Abs(A/B);
  AErr = TMath::Power((SigA/A),2);
  BErr = TMath::Power((SigB/B),2);
  PartErrSq = AErr + BErr;
  
  Error = f * TMath::Sqrt(PartErrSq);
  

  return Error;
} 

void betaTree_Plane2::Loop()
{
//   In a ROOT session, you can do:
//      root> .L betaTree_Plane2.C
//      root> betaTree_Plane2 t
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
  //vectors of fit functions
  std::vector<TF1*> XFits;
  std::vector<TF1*> YFits;
  std::vector<TF1*> ZFits;
  std::vector<TF1*> RecoFits;
  std::vector<TF1*> TrueFits;
  //vector of resolution graphs
  std::vector<TH1D*> Means;
  std::vector<TH1D*> Sigmas;
  std::vector<TH1D*> Resolutions;
  //We'll want 10 bins in each variable and 10 fits
  EnergyResolutionVsBetaX.resize(10);
  EnergyResolutionVsBetaY.resize(10);
  EnergyResolutionVsBetaZ.resize(10);
  EnergyResolutionVsBetaReco.resize(10);
  EnergyResolutionVsBetaTrue.resize(10);
  XFits.resize(10);
  YFits.resize(10);
  ZFits.resize(10);
  RecoFits.resize(10);
  TrueFits.resize(8);
  Means.resize(5);
  Sigmas.resize(5);
  Resolutions.resize(5);
  //Our 10 bins will need 11 bin edges
  double xBins[11] = {0, 26, 52, 78, 104, 130, 156, 182, 208, 234, 260};
  double xMid[10] = {13, 39, 65, 91, 117, 143, 169, 195, 221, 247};
  double yBins[11] = {-120, -96, -72, -48, -24, 0, 24, 48, 72, 96, 120};
  double yMid[10] = {-108, -84, -60, -36, -12, 12, 36, 60, 84, 108};
  double zBins[11] = {0, 105, 210, 315, 420, 525, 630, 735, 840, 945, 1050};
  double zMid[10] = {52.5, 157.5, 262.5, 367.5, 472.5, 577.5, 682.5, 787.5, 892.5, 997.5};
  double recoBins[11] = {0.5, 0.85, 1.2, 1.55, 1.9, 2.25, 2.6, 2.95, 3.3, 3.65, 4};
  double recoMid[10] = {0.675, 1.025, 1.375, 1.725, 2.075, 2.425, 2.775, 3.125, 3.475, 3.825};
  double trueBins[11] = {0.5, 0.85, 1.2, 1.55, 1.9, 2.25, 2.6, 2.95, 3.3, 3.65, 4};
  double trueMid[8] = {0.675, 1.025, 1.375, 1.725, 2.075, 2.425, 2.775, 3.125};
  double RecoMean[10] = {0};
  double TrueMean[8] = {0};
  double XMean[10] = {0};
  double YMean[10] = {0};
  double ZMean[10] = {0};
  double RecoMeanErr[8] = {0};
  double TrueMeanErr[8] = {0};
  double XMeanErr[10] = {0};
  double YMeanErr[10] = {0};
  double ZMeanErr[10] = {0};
  double RecoSigma[10] = {0};
  double TrueSigma[8] = {0};
  double XSigma[10] = {0};
  double YSigma[10] = {0};
  double ZSigma[10] = {0};
  double RecoSigmaErr[8] = {0};
  double TrueSigmaErr[8] = {0};
  double XSigmaErr[10] = {0};
  double YSigmaErr[10] = {0};
  double ZSigmaErr[10] = {0};
  double RecoReso[10] = {0};
  double TrueReso[8] = {0};
  double XReso[10] = {0};
  double YReso[10] = {0};
  double ZReso[10] = {0};
  double RecoResoErr[8] = {0};
  double TrueResoErr[8] = {0};
  double XResoErr[10] = {0};
  double YResoErr[10] = {0};
  double ZResoErr[10] = {0};
  double BinNum[10] = {0,1,2,3,4,5,6,7,8,9};
  double BinNumTrue[8] = {0,1,2,3,4,5,6,7};

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
    if(i > 6){
      EnergyResolutionVsBetaReco[i] = new TH1D(Form("EnergyResolution_in_BetaReco_bin_%d", i), ";Energy Resolution;Counts",100, -2, 3);
    }
  }
  
  for(int i = 0;i < EnergyResolutionVsBetaTrue.size(); i++){
    //initialize each of these plots
    EnergyResolutionVsBetaTrue[i] = new TH1D(Form("EnergyResolution_in_BetaTrue_bin_%d", i), ";Energy Resolution;Counts",100, -2, 2);
    if(i > 7){
      EnergyResolutionVsBetaTrue[i] = new TH1D(Form("EnergyResolution_in_BetaTrue_bin_%d", i), ";Energy Resolution;Counts",100, 0, 4);
    }
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
      
      if(betaRecoEnergy_plane2 == 0) continue;
      if(betaRecoEnergy_plane2/100 > 4 || betaRecoEnergy_plane2/100 < 0.5) continue;

      double energyResolution = ((betaRecoEnergy_plane2/100)-betaTrueEnergy)/betaTrueEnergy;

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
      
	if(betaRecoEnergy_plane2/100 > recoBins[recobin] && betaRecoEnergy_plane2/100 <= recoBins[recobin+1]){
	  EnergyResolutionVsBetaReco[recobin]->Fill(energyResolution);

	}
	
      }

      for(int truebin = 0; truebin < 10; truebin++){
      
	if(betaTrueEnergy > trueBins[truebin] && betaTrueEnergy <= trueBins[truebin+1]){
	  EnergyResolutionVsBetaTrue[truebin]->Fill(energyResolution);

	}
	
      }
   
   }
   
   //gStyle->SetOptStat(0);

   //initialize and set parameters for the fits
   
   for(int i=0; i < XFits.size(); i++){
     XFits[i] = new TF1(Form("XFit_bin_%d", i),"gaus",-0.7,0.7);
     XFits[i]->SetParameters(EnergyResolutionVsBetaX[i]->GetMaximum(),EnergyResolutionVsBetaX[i]->GetMean());
   }
   
   for(int i=0; i < YFits.size(); i++){
     YFits[i] = new TF1(Form("YFit_bin_%d",i),"gaus",-0.7,0.7);
     YFits[i]->SetParameters(EnergyResolutionVsBetaY[i]->GetMaximum(),EnergyResolutionVsBetaY[i]->GetMean());
   }
   
   for(int i=0; i < ZFits.size(); i++){
     ZFits[i] = new TF1(Form("ZFit_bin_%d",i),"gaus",-0.7,0.7);
     ZFits[i]->SetParameters(EnergyResolutionVsBetaZ[i]->GetMaximum(),EnergyResolutionVsBetaZ[i]->GetMean());
   }
   
   for(int i=0; i < RecoFits.size(); i++){
     if(i<8){
       RecoFits[i] = new TF1(Form("RecoFit_bin_%d",i),"gaus",-0.25,0.25);
       RecoFits[i]->SetParameters(EnergyResolutionVsBetaReco[i]->GetMaximum(),EnergyResolutionVsBetaReco[i]->GetMean());
     }
     else{
       RecoFits[i] = new TF1(Form("RecoFit_bin_%d",i),"gaus",-0.1,1.5);
       RecoFits[i]->SetParameters(EnergyResolutionVsBetaReco[i]->GetMaximum(),0.2);
     }
   }
   
   for(int i=0; i < TrueFits.size(); i++){
     TrueFits[i] = new TF1(Form("TrueFit_bin_%d",i),"gaus",-0.25,0.25);
     TrueFits[i]->SetParameters(EnergyResolutionVsBetaTrue[i]->GetMaximum(),EnergyResolutionVsBetaTrue[i]->GetMean());
   }
   
   std::vector<TCanvas*> c; c.resize(50);
   
   for(int i = 0; i < 10; i++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaX[i]->SetTitle(Form("Plane 2: Energy Resolution in Beta X bin %d [%5f,%5f]", i, xBins[i], xBins[i+1]));
     EnergyResolutionVsBetaX[i]->Fit(Form("XFit_bin_%d",i),"R");
     TF1 *fit = EnergyResolutionVsBetaX[i]->GetFunction(Form("XFit_bin_%d",i));
     XMean[i] = fit->GetParameter(1);
     XSigma[i] = fit->GetParameter(2);
     XReso[i] = (fit->GetParameter(2))/(fit->GetParameter(1));
     XMeanErr[i] = fit->GetParError(1);
     XSigmaErr[i] = fit->GetParError(2);
     XResoErr[i] = ErrorProp(XSigma[i], XMean[i], XSigmaErr[i], XMeanErr[i]);
     EnergyResolutionVsBetaX[i]->Draw();
     c[i]->SaveAs(Form("EnergyResolution_in_BetaX_bin_%d.png", i));
   }

   
   
   for(int i = 10, j = 0; i < 20 && j < 10; i++, j++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaY[j]->SetTitle(Form("Plane 2: Energy Resolution in Beta Y bin %d [%5f,%5f]", j, yBins[j], yBins[j+1]));
     EnergyResolutionVsBetaY[j]->Fit(Form("YFit_bin_%d",j),"R");
     TF1 *fit = EnergyResolutionVsBetaY[j]->GetFunction(Form("YFit_bin_%d",j));
     YMean[j] = fit->GetParameter(1);
     YSigma[j] = fit->GetParameter(2);
     YReso[j] = (fit->GetParameter(2))/(fit->GetParameter(1));
     YMeanErr[j] = fit->GetParError(1);
     YSigmaErr[j] = fit->GetParError(2);
     YResoErr[j] = ErrorProp(YSigma[j], YMean[j], YSigmaErr[j], YMeanErr[j]);
     EnergyResolutionVsBetaY[j]->Draw();
     c[i]->SaveAs(Form("EnergyResolution_in_BetaY_bin_%d.png", j));
   }
   
   for(int i = 20, j = 0; i < 30 && j < 10; i++, j++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaZ[j]->SetTitle(Form("Plane 2: Energy Resolution in Beta Z bin %d [%5f,%5f]", j, zBins[j], zBins[j+1]));
     EnergyResolutionVsBetaZ[j]->Fit(Form("ZFit_bin_%d",j),"R");
     TF1 *fit = EnergyResolutionVsBetaZ[j]->GetFunction(Form("ZFit_bin_%d",j));
     ZMean[j] = fit->GetParameter(1);
     ZSigma[j] = fit->GetParameter(2);
     ZReso[j] = (fit->GetParameter(2))/(fit->GetParameter(1));
     ZMeanErr[j] = fit->GetParError(1);
     ZSigmaErr[j] = fit->GetParError(2);
     ZResoErr[j] = ErrorProp(ZSigma[j], ZMean[j], ZSigmaErr[j], ZMeanErr[j]);
     EnergyResolutionVsBetaZ[j]->Draw();
     c[i]->SaveAs(Form("EnergyResolution_in_BetaZ_bin_%d.png", j));
   }
   
   for(int i = 30, j = 0; i < 40 && j < 10; i++, j++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaReco[j]->SetTitle(Form("Plane 2: Energy Resolution in Beta Reco bin %d [%5f,%5f]", j, recoBins[j], recoBins[j+1]));
     EnergyResolutionVsBetaReco[j]->Sumw2();
     EnergyResolutionVsBetaReco[j]->Fit(Form("RecoFit_bin_%d",j),"R");
     TF1 *fit = EnergyResolutionVsBetaReco[j]->GetFunction(Form("RecoFit_bin_%d",j));
     RecoMean[j] = fit->GetParameter(1);
     RecoSigma[j] = fit->GetParameter(2);
     RecoReso[j] = (fit->GetParameter(2))/(fit->GetParameter(1));
     if(j < 8){
       RecoMeanErr[j] = fit->GetParError(1);
       RecoSigmaErr[j] = fit->GetParError(2);
       RecoResoErr[j] = ErrorProp(RecoSigma[j], RecoMean[j], RecoSigmaErr[j], RecoMeanErr[j]);
     }
     EnergyResolutionVsBetaReco[j]->Draw();
     c[i]->SaveAs(Form("EnergyResolution_in_BetaReco_bin_%d.png", j));
   }
  
   for(int i = 40, j = 0; i < 48 && j < 8; i++, j++){
     c[i] = new TCanvas();
     c[i]->cd();
     EnergyResolutionVsBetaTrue[j]->SetTitle(Form("Plane 2: Energy Resolution in Beta True bin %d [%5f,%5f]", j, trueBins[j], trueBins[j+1]));
     EnergyResolutionVsBetaTrue[j]->Fit(Form("TrueFit_bin_%d",j),"R");
     TF1 *fit = EnergyResolutionVsBetaTrue[j]->GetFunction(Form("TrueFit_bin_%d",j));
     TrueMean[j] = fit->GetParameter(1);
     TrueSigma[j] = fit->GetParameter(2);
     TrueReso[j] = (fit->GetParameter(2))/(fit->GetParameter(1));
     TrueMeanErr[j] = fit->GetParError(1);
     TrueSigmaErr[j] = fit->GetParError(2);
     TrueResoErr[j] = ErrorProp(TrueSigma[j], TrueMean[j], TrueSigmaErr[j], TrueMeanErr[j]);
     EnergyResolutionVsBetaTrue[j]->Draw();
     c[i]->SaveAs(Form("EnergyResolution_in_BetaTrue_bin_%d.png", j));
   }
   
   Means[0] = new TH1D("Means_X",";Creation X (cm);Mean",10,0,260);
   Means[1] = new TH1D("Means_Y",";Creation Y (cm);Mean",10,-120,120);
   Means[2] = new TH1D("Means_Z",";Creation Z (cm);Mean",10,0,1050);
   Means[3] = new TH1D("Means_Reco",";Reconstructed Energy (~MeV);Mean",8,0.5,3.3);
   Means[4] = new TH1D("Means_True",";True Energy (MeV);Mean",8,0.5,3.3);

   Sigmas[0] = new TH1D("Sigmas_X",";Creation X (cm);Width of gaussian fit",10,0,260);
   Sigmas[1] = new TH1D("Sigmas_Y",";Creation Y (cm);Width of gaussian fit",10,-120,120);
   Sigmas[2] = new TH1D("Sigmas_Z",";Creation Z (cm);Width of gaussian fit",10,0,1050);
   Sigmas[3] = new TH1D("Sigmas_Reco",";Reconstructed Energy (~MeV);Width of gaussian fit",8,0.5,3.3);
   Sigmas[4] = new TH1D("Sigmas_True",";True Energy (MeV);Width of gaussian fit",8,0.5,3.3);

   Resolutions[0] = new TH1D("Resos_X",";Creation X (cm);Resolution",10,0,260);
   Resolutions[1] = new TH1D("Resos_Y",";Creation Y (cm);Resolution",10,-120,120);
   Resolutions[2] = new TH1D("Resos_Z",";Creation Z (cm);Resolution",10,0,1050);
   Resolutions[3] = new TH1D("Resos_Reco",";Reconstructed Energy (~MeV);Resolution",8,0.5,3.3);
   Resolutions[4] = new TH1D("Resos_True",";True Energy (MeV);Resolution",8,0.5,3.3);
   
   TF1* fitSig = new TF1("fitSig","x*sqrt(pow([0]/sqrt(x),2) + pow([1]/x,2) + pow([2],2))",0.5,3.3);
   fitSig->SetParameter(0,0.01);
   fitSig->SetParameter(1,0.01);
   fitSig->SetParameter(2,0.01);

   for(int i=0; i < 10; i++){
     Means[0]->Fill(xMid[i],XMean[i]);
     Means[1]->Fill(yMid[i],YMean[i]);
     Means[2]->Fill(zMid[i],ZMean[i]);
     
     Sigmas[0]->Fill(xMid[i],XSigma[i]);
     Sigmas[1]->Fill(yMid[i],YSigma[i]);
     Sigmas[2]->Fill(zMid[i],ZSigma[i]);

     Resolutions[0]->Fill(xMid[i],XReso[i]);
     Resolutions[1]->Fill(yMid[i],YReso[i]);
     Resolutions[2]->Fill(zMid[i],ZReso[i]);

     if(i < 8){
       Means[3]->Fill(recoMid[i],RecoMean[i]);
       Sigmas[3]->Fill(recoMid[i],RecoSigma[i]);
       Resolutions[3]->Fill(recoMid[i],RecoReso[i]);
       Means[4]->Fill(trueMid[i],TrueMean[i]);
       Sigmas[4]->Fill(trueMid[i],TrueSigma[i]);
       Resolutions[4]->Fill(trueMid[i],TrueReso[i]);
     }

   }

   Means[0]->SetError(XMeanErr);
   Means[1]->SetError(YMeanErr);
   Means[2]->SetError(ZMeanErr);
   Means[3]->SetError(RecoMeanErr);
   
   Sigmas[0]->SetError(XSigmaErr);
   Sigmas[1]->SetError(YSigmaErr);
   Sigmas[2]->SetError(ZSigmaErr);
   Sigmas[3]->SetError(RecoSigmaErr);
   
   Resolutions[0]->SetError(XResoErr);
   Resolutions[1]->SetError(YResoErr);
   Resolutions[2]->SetError(ZResoErr);
   Resolutions[3]->SetError(RecoResoErr);
   
   Means[4]->SetError(TrueMeanErr);
   Sigmas[4]->SetError(TrueSigmaErr);
   Resolutions[4]->SetError(TrueResoErr);


   std::vector<TCanvas*> b; b.resize(15);
   const char *type[5] = {"X","Y","Z","Reco","True"}; 

   for(int i=0; i < 5; i++){
     b[i] = new TCanvas();
     b[i]->cd();
     Means[i]->SetTitle(Form("Mean per bin for %s",type[i]));
     Means[i]->SetMarkerStyle(kFullCrossX);
     Means[i]->SetMarkerSize(1);
     Means[i]->Draw("EX0P");
     b[i]->SaveAs(Form("Mean_perBin_%d.png",i));
   }

   for(int i=0, j=5; i < 5, j < 10; i++, j++){
     b[j] = new TCanvas();
     b[j]->cd();
     Sigmas[i]->SetTitle(Form("Width of gaussian fit per bin for %s",type[i]));
     Sigmas[i]->SetMarkerStyle(kFullCrossX);
     Sigmas[i]->SetMarkerSize(1);
     if(i == 3){Sigmas[i]->Fit("fitSig","R");}
     Sigmas[i]->Draw("EX0P");
     b[j]->SaveAs(Form("Sigma_perBin_%d.png",i));
   }
  
   for(int i=0, j=10; i < 5, j < 15; i++, j++){
     b[j] = new TCanvas();
     b[j]->cd();
     Resolutions[i]->SetTitle(Form("Resolution per bin for %s",type[i]));
     Resolutions[i]->SetMarkerStyle(kFullCrossX);
     Resolutions[i]->SetMarkerSize(1);
     Resolutions[i]->Draw("EX0P");
     b[j]->SaveAs(Form("Reso_perBin_%d.png",i));
   }

}

