//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 21 15:26:02 2021 by ROOT version 6.12/06
// from TTree betaTree/ForEachTruthBeta
// found on file: outfile_mc_TESTCOMPARE.root
//////////////////////////////////////////////////////////

#ifndef betaTree_Plane0_h
#define betaTree_Plane0_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class betaTree_Plane0 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        betaTrueEnergy;
   Double_t        betaRecoEnergy_plane0;
   Double_t        betaCreationTime;
   Double_t        betaCreationX;
   Double_t        betaCreationY;
   Double_t        betaCreationZ;
   Double_t        betaPrecision;
   Double_t        betaMomentumX;
   Double_t        betaMomentumY;
   Double_t        betaMomentumZ;
   Double_t        betaMomentumT;

   // List of branches
   TBranch        *b_betaTrueEnergy;   //!
   TBranch        *b_betaRecoEnergy_plane0;   //!
   TBranch        *b_betaCreationTime;   //!
   TBranch        *b_betaCreationX;   //!
   TBranch        *b_betaCreationY;   //!
   TBranch        *b_betaCreationZ;   //!
   TBranch        *b_betaPrecision;   //!
   TBranch        *b_betaMomentumX;   //!
   TBranch        *b_betaMomentumY;   //!
   TBranch        *b_betaMomentumZ;   //!
   TBranch        *b_betaMomentumT;   //!

   betaTree_Plane0(TTree *tree=0);
   virtual ~betaTree_Plane0();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef betaTree_Plane0_cxx
betaTree_Plane0::betaTree_Plane0(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("outfile_mc_TESTCOMPARE.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("outfile_mc_TESTCOMPARE.root");
      }
      f->GetObject("betaTree",tree);

   }
   Init(tree);
}

betaTree_Plane0::~betaTree_Plane0()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t betaTree_Plane0::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t betaTree_Plane0::LoadTree(Long64_t entry)
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

void betaTree_Plane0::Init(TTree *tree)
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
   fChain->SetBranchAddress("betaRecoEnergy_plane0", &betaRecoEnergy_plane0, &b_betaRecoEnergy_plane0);
   fChain->SetBranchAddress("betaCreationTime", &betaCreationTime, &b_betaCreationTime);
   fChain->SetBranchAddress("betaCreationX", &betaCreationX, &b_betaCreationX);
   fChain->SetBranchAddress("betaCreationY", &betaCreationY, &b_betaCreationY);
   fChain->SetBranchAddress("betaCreationZ", &betaCreationZ, &b_betaCreationZ);
   fChain->SetBranchAddress("betaPrecision", &betaPrecision, &b_betaPrecision);
   fChain->SetBranchAddress("betaMomentumX", &betaMomentumX, &b_betaMomentumX);
   fChain->SetBranchAddress("betaMomentumY", &betaMomentumY, &b_betaMomentumY);
   fChain->SetBranchAddress("betaMomentumZ", &betaMomentumZ, &b_betaMomentumZ);
   fChain->SetBranchAddress("betaMomentumT", &betaMomentumT, &b_betaMomentumT);
   Notify();
}

Bool_t betaTree_Plane0::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void betaTree_Plane0::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t betaTree_Plane0::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef betaTree_Plane0_cxx
