#define ClusterTree_cxx
#include "ClusterTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void ClusterTree::Loop()
{
//   In a ROOT session, you can do:
//      root> .L ClusterTree.C
//      root> ClusterTree t
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
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   int matchedToBeta = 0;
   int matchedToAlpha = 0;
   int matchedToBetaAndAlpha = 0;


   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      std::cout << "cluster size : " << cluster_x_vec->size() << std::endl;

      for(int clus = 0; clus < cluster_x_vec->size(); clus++){
	if(cluster_IsTrueBeta_vec->at(clus) == true)
	  matchedToBeta++;
 
	if(cluster_IsTrueAlpha_vec->at(clus) == true)
	  matchedToAlpha++;
    
	if(cluster_IsTrueBeta_vec->at(clus) == true && cluster_IsTrueAlpha_vec->at(clus) == true)
	  matchedToBetaAndAlpha++;
	
	
      }
   }

   std::cout << "N beta: " << matchedToBeta << ", N alpha: " << matchedToAlpha << ", N both : " << matchedToBetaAndAlpha << std::endl; 

}
