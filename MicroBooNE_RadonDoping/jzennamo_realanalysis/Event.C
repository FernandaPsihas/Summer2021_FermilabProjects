#define Event_cxx
#include "Event.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <vector>
#include <iostream>


void Event::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Event.C
//      root> Event t
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

  TH1D* beta_cluster_z = new TH1D("beta_cluster_z", "", 1000, 0, 1036);
  TH1D* beta_to_alpha_cluster_x_distance = new TH1D("beta_to_alpha_cluster_x_distance", "", 100, -256, 256);
  
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      //std::cout << "ClusterPDG size : " << ClusterPDG->size() << std::endl;
      
      for(int b = 0; b < ClusterPDG->size(); b++){
	if(ClusterPDG->at(b) != 11) continue;
	if(ClusterPrimary->at(b) == false) continue;
	if(cluster_plane->at(b) != 2) continue;

	//std::cout << "Beta!" << std::endl;
	
	beta_cluster_z->Fill(cluster_z->at(b));
	
	for(int a = 0; a < ClusterPDG->size(); a++){
	  if(ClusterPDG->at(a) != 1000020040) continue;
	  //if(ClusterPrimary->at(a) == false) continue;
	  //if(cluster_plane->at(a) != 2) continue;
	  
	  std::cout << "Alpha!" << std::endl;

	  //	  if(cluster_x->at(b) > cluster_x->at(a)) continue;
	  // if(abs(cluster_z->at(a) - cluster_z->at(b)) > 3) continue; 

	  double deltaX = cluster_x->at(a) - cluster_x->at(b);
	  
	  beta_to_alpha_cluster_x_distance->Fill(deltaX);
	  
	  

	}//iterate over alpha

      }//iterate over beta


   }// events

   TCanvas* c1 = new TCanvas(); 
   c1->cd();
   beta_to_alpha_cluster_x_distance->Draw();

   TCanvas* c2 = new TCanvas(); 
   c2->cd();
   beta_cluster_z->Draw();
}
