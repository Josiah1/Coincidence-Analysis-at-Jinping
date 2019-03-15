//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 16 23:31:39 2018 by ROOT version 6.10/08
// from TTree Event/Analysis Tree
// found on file: 331.ostw.root
//////////////////////////////////////////////////////////

#ifndef Event_h
#define Event_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class Event {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           Run;
   Int_t           Det;
   Double_t        T2PrevMu;
   Double_t        T2PrevPlMu;
   Double_t        T2PrevAdMu;
   Double_t        T2PrevShMu;
   Double_t        T2PrevNetMu;
   Int_t           Fold;
   Int_t           FileNum[5];   //[Fold]
   Int_t           TrigNum[5];   //[Fold]
   Int_t           TrigSec[5];   //[Fold]
   Int_t           TrigNano[5];   //[Fold]
   Double_t        dis[5];   //[Fold]
   Double_t        E[5];   //[Fold]
   Double_t        X[5];   //[Fold]
   Double_t        Y[5];   //[Fold]
   Double_t        Z[5];   //[Fold]
   Double_t        D2First[5];   //[Fold]
   Double_t        T2PrevSubEvt[5];   //[Fold]
   Double_t        Quadrant[5];   //[Fold]
   Double_t        QMax2Sum[5];   //[Fold]

   // List of branches
   TBranch        *b_Run;   //!
   TBranch        *b_Det;   //!
   TBranch        *b_T2PrevMu;   //!
   TBranch        *b_T2PrevPlMu;   //!
   TBranch        *b_T2PrevAdMu;   //!
   TBranch        *b_T2PrevShMu;   //!
   TBranch        *b_T2PrevNetMu;   //!
   TBranch        *b_Fold;   //!
   TBranch        *b_FileNum;   //!
   TBranch        *b_TrigNum;   //!
   TBranch        *b_TrigSec;   //!
   TBranch        *b_TrigNano;   //!
   TBranch        *b_dis;   //!
   TBranch        *b_E;   //!
   TBranch        *b_X;   //!
   TBranch        *b_Y;   //!
   TBranch        *b_Z;   //!
   TBranch        *b_D2First;   //!
   TBranch        *b_T2PrevSubEvt;   //!
   TBranch        *b_Quadrant;   //!
   TBranch        *b_QMax2Sum;   //!

   Event(TTree *tree=0);
   virtual ~Event();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Event_cxx
Event::Event(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("331.ostw.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("331.ostw.root");
      }
      f->GetObject("Event",tree);

   }
   Init(tree);
}

Event::~Event()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Event::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Event::LoadTree(Long64_t entry)
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

void Event::Init(TTree *tree)
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

   fChain->SetBranchAddress("Run", &Run, &b_Run);
   fChain->SetBranchAddress("Det", &Det, &b_Det);
   fChain->SetBranchAddress("T2PrevMu", &T2PrevMu, &b_T2PrevMu);
   fChain->SetBranchAddress("T2PrevPlMu", &T2PrevPlMu, &b_T2PrevPlMu);
   fChain->SetBranchAddress("T2PrevAdMu", &T2PrevAdMu, &b_T2PrevAdMu);
   fChain->SetBranchAddress("T2PrevShMu", &T2PrevShMu, &b_T2PrevShMu);
   fChain->SetBranchAddress("T2PrevNetMu", &T2PrevNetMu, &b_T2PrevNetMu);
   fChain->SetBranchAddress("Fold", &Fold, &b_Fold);
   fChain->SetBranchAddress("FileNum", FileNum, &b_FileNum);
   fChain->SetBranchAddress("TrigNum", TrigNum, &b_TrigNum);
   fChain->SetBranchAddress("TrigSec", TrigSec, &b_TrigSec);
   fChain->SetBranchAddress("TrigNano", TrigNano, &b_TrigNano);
   fChain->SetBranchAddress("dis", dis, &b_dis);
   fChain->SetBranchAddress("E", E, &b_E);
   fChain->SetBranchAddress("X", X, &b_X);
   fChain->SetBranchAddress("Y", Y, &b_Y);
   fChain->SetBranchAddress("Z", Z, &b_Z);
   fChain->SetBranchAddress("D2First", D2First, &b_D2First);
   fChain->SetBranchAddress("T2PrevSubEvt", T2PrevSubEvt, &b_T2PrevSubEvt);
   fChain->SetBranchAddress("Quadrant", Quadrant, &b_Quadrant);
   fChain->SetBranchAddress("QMax2Sum", QMax2Sum, &b_QMax2Sum);
   Notify();
}

Bool_t Event::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Event::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Event::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Event_cxx
