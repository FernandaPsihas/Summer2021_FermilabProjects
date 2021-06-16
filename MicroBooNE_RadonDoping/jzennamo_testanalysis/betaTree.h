//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jun  9 14:54:40 2021 by ROOT version 6.12/06
// from TTree betaTree/ForEachTruthBeta
// found on file: ../jzennamo_testreco/outfile_mc_TEST.root
//////////////////////////////////////////////////////////

#ifndef betaTree_h
#define betaTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class betaTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        betaTrueEnergy;
   Double_t        betaRecoEnergy_plane2;
   Double_t        betaCreationTime;
   Double_t        betaCreationX;
   Double_t        betaCreationY;
   Double_t        betaCreationZ;

   // List of branches
   TBranch        *b_betaTrueEnergy;   //!
   TBranch        *b_betaRecoEnergy_plane2;   //!
   TBranch        *b_betaCreationTime;   //!
   TBranch        *b_betaCreationX;   //!
   TBranch        *b_betaCreationY;   //!
   TBranch        *b_betaCreationZ;   //!

   betaTree(TTree *tree=0);
   virtual ~betaTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef betaTree_cxx
betaTree::betaTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../jzennamo_testreco/outfile_mc_TEST.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../jzennamo_testreco/outfile_mc_TEST.root");
      }
      f->GetObject("betaTree",tree);

   }
   Init(tree);
}

betaTree::~betaTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t betaTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t betaTree::LoadTree(Long64_t entry)
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

void betaTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("betaTrueEnergy", &betaTrueEnergy, &b_betaTrueEnergy);
   fChain->SetBranchAddress("betaRecoEnergy_plane2", &betaRecoEnergy_plane2, &b_betaRecoEnergy_plane2);
   fChain->SetBranchAddress("betaCreationTime", &betaCreationTime, &b_betaCreationTime);
   fChain->SetBranchAddress("betaCreationX", &betaCreationX, &b_betaCreationX);
   fChain->SetBranchAddress("betaCreationY", &betaCreationY, &b_betaCreationY);
   fChain->SetBranchAddress("betaCreationZ", &betaCreationZ, &b_betaCreationZ);
   Notify();
}

Bool_t betaTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void betaTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t betaTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef betaTree_cxx
