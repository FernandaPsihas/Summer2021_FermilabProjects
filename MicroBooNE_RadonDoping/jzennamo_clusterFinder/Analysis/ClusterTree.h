//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Aug 31 10:20:50 2021 by ROOT version 6.12/06
// from TTree ClusterTree/All information saved by cluster
// found on file: ../outfile_mc_TEST.root
//////////////////////////////////////////////////////////

#ifndef ClusterTree_h
#define ClusterTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "vector"

class ClusterTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   vector<double>  *cluster_x_vec;
   vector<double>  *cluster_z_vec;
   vector<double>  *cluster_start_wire;
   vector<double>  *cluster_end_wire;
   vector<double>  *cluster_energy;
   vector<double>  *cluster_avg_energy;
   vector<double>  *cluster_std_energy;
   vector<double>  *cluster_multi_hit_density;
   vector<bool>    *cluster_IsTrueBeta_vec;
   vector<bool>    *cluster_IsTrueAlpha_vec;
   vector<bool>    *cluster_MatchedToMCpart_vec;
   vector<vector<double> > *hit_Q_for_cluster_vec;
   vector<vector<double> > *hit_sigma_for_cluster_vec;
   vector<vector<double> > *hit_GOF_for_cluster_vec;
   vector<vector<double> > *hit_mult_for_cluster_vec;
   vector<vector<vector<double> > > *mcpart_pdg_for_hit_for_cluster_vec;
   vector<vector<vector<double> > > *mcpart_primary_for_hit_for_cluster_vec;
   vector<vector<vector<double> > > *mcpart_trueX_for_hit_for_cluster_vec;
   vector<vector<vector<double> > > *mcpart_trueY_for_hit_for_cluster_vec;
   vector<vector<vector<double> > > *mcpart_trueZ_for_hit_for_cluster_vec;
   vector<vector<vector<double> > > *mcpart_frac_for_hit_for_cluster_vec;
   vector<vector<vector<double> > > *mcpart_matched_energy_for_hit_for_cluster_vec;

   // List of branches
   TBranch        *b_cluster_x_vec;   //!
   TBranch        *b_cluster_z_vec;   //!
   TBranch        *b_cluster_start_wire;   //!
   TBranch        *b_cluster_end_wire;   //!
   TBranch        *b_cluster_energy;   //!
   TBranch        *b_cluster_avg_energy;   //!
   TBranch        *b_cluster_std_energy;   //!
   TBranch        *b_cluster_multi_hit_density;   //!
   TBranch        *b_cluster_IsTrueBeta_vec;   //!
   TBranch        *b_cluster_IsTrueAlpha_vec;   //!
   TBranch        *b_cluster_MatchedToMCpart_vec;   //!
   TBranch        *b_hit_Q_for_cluster_vec;   //!
   TBranch        *b_hit_sigma_for_cluster_vec;   //!
   TBranch        *b_hit_GOF_for_cluster_vec;   //!
   TBranch        *b_hit_mult_for_cluster_vec;   //!
   TBranch        *b_mcpart_pdg_for_hit_for_cluster_vec;   //!
   TBranch        *b_mcpart_primary_for_hit_for_cluster_vec;   //!
   TBranch        *b_mcpart_trueX_for_hit_for_cluster_vec;   //!
   TBranch        *b_mcpart_trueY_for_hit_for_cluster_vec;   //!
   TBranch        *b_mcpart_trueZ_for_hit_for_cluster_vec;   //!
   TBranch        *b_mcpart_frac_for_hit_for_cluster_vec;   //!
   TBranch        *b_mcpart_matched_energy_for_hit_for_cluster_vec;   //!

   ClusterTree(TTree *tree=0);
   virtual ~ClusterTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ClusterTree_cxx
ClusterTree::ClusterTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../outfile_mc_TEST.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../outfile_mc_TEST.root");
      }
      f->GetObject("ClusterTree",tree);

   }
   Init(tree);
}

ClusterTree::~ClusterTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ClusterTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ClusterTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ClusterTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   cluster_x_vec = 0;
   cluster_z_vec = 0;
   cluster_start_wire = 0;
   cluster_end_wire = 0;
   cluster_energy = 0;
   cluster_avg_energy = 0;
   cluster_std_energy = 0;
   cluster_multi_hit_density = 0;
   cluster_IsTrueBeta_vec = 0;
   cluster_IsTrueAlpha_vec = 0;
   cluster_MatchedToMCpart_vec = 0;
   hit_Q_for_cluster_vec = 0;
   hit_sigma_for_cluster_vec = 0;
   hit_GOF_for_cluster_vec = 0;
   hit_mult_for_cluster_vec = 0;
   mcpart_pdg_for_hit_for_cluster_vec = 0;
   mcpart_primary_for_hit_for_cluster_vec = 0;
   mcpart_trueX_for_hit_for_cluster_vec = 0;
   mcpart_trueY_for_hit_for_cluster_vec = 0;
   mcpart_trueZ_for_hit_for_cluster_vec = 0;
   mcpart_frac_for_hit_for_cluster_vec = 0;
   mcpart_matched_energy_for_hit_for_cluster_vec = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("cluster_x_vec", &cluster_x_vec, &b_cluster_x_vec);
   fChain->SetBranchAddress("cluster_z_vec", &cluster_z_vec, &b_cluster_z_vec);
   fChain->SetBranchAddress("cluster_start_wire", &cluster_start_wire, &b_cluster_start_wire);
   fChain->SetBranchAddress("cluster_end_wire", &cluster_end_wire, &b_cluster_end_wire);
   fChain->SetBranchAddress("cluster_energy", &cluster_energy, &b_cluster_energy);
   fChain->SetBranchAddress("cluster_avg_energy", &cluster_avg_energy, &b_cluster_avg_energy);
   fChain->SetBranchAddress("cluster_std_energy", &cluster_std_energy, &b_cluster_std_energy);
   fChain->SetBranchAddress("cluster_multi_hit_density", &cluster_multi_hit_density, &b_cluster_multi_hit_density);
   fChain->SetBranchAddress("cluster_IsTrueBeta_vec", &cluster_IsTrueBeta_vec, &b_cluster_IsTrueBeta_vec);
   fChain->SetBranchAddress("cluster_IsTrueAlpha_vec", &cluster_IsTrueAlpha_vec, &b_cluster_IsTrueAlpha_vec);
   fChain->SetBranchAddress("cluster_MatchedToMCpart_vec", &cluster_MatchedToMCpart_vec, &b_cluster_MatchedToMCpart_vec);
   fChain->SetBranchAddress("hit_Q_for_cluster_vec", &hit_Q_for_cluster_vec, &b_hit_Q_for_cluster_vec);
   fChain->SetBranchAddress("hit_sigma_for_cluster_vec", &hit_sigma_for_cluster_vec, &b_hit_sigma_for_cluster_vec);
   fChain->SetBranchAddress("hit_GOF_for_cluster_vec", &hit_GOF_for_cluster_vec, &b_hit_GOF_for_cluster_vec);
   fChain->SetBranchAddress("hit_mult_for_cluster_vec", &hit_mult_for_cluster_vec, &b_hit_mult_for_cluster_vec);
   fChain->SetBranchAddress("mcpart_pdg_for_hit_for_cluster_vec", &mcpart_pdg_for_hit_for_cluster_vec, &b_mcpart_pdg_for_hit_for_cluster_vec);
   fChain->SetBranchAddress("mcpart_primary_for_hit_for_cluster_vec", &mcpart_primary_for_hit_for_cluster_vec, &b_mcpart_primary_for_hit_for_cluster_vec);
   fChain->SetBranchAddress("mcpart_trueX_for_hit_for_cluster_vec", &mcpart_trueX_for_hit_for_cluster_vec, &b_mcpart_trueX_for_hit_for_cluster_vec);
   fChain->SetBranchAddress("mcpart_trueY_for_hit_for_cluster_vec", &mcpart_trueY_for_hit_for_cluster_vec, &b_mcpart_trueY_for_hit_for_cluster_vec);
   fChain->SetBranchAddress("mcpart_trueZ_for_hit_for_cluster_vec", &mcpart_trueZ_for_hit_for_cluster_vec, &b_mcpart_trueZ_for_hit_for_cluster_vec);
   fChain->SetBranchAddress("mcpart_frac_for_hit_for_cluster_vec", &mcpart_frac_for_hit_for_cluster_vec, &b_mcpart_frac_for_hit_for_cluster_vec);
   fChain->SetBranchAddress("mcpart_matched_energy_for_hit_for_cluster_vec", &mcpart_matched_energy_for_hit_for_cluster_vec, &b_mcpart_matched_energy_for_hit_for_cluster_vec);
   Notify();
}

Bool_t ClusterTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ClusterTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ClusterTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ClusterTree_cxx
