//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat May 26 17:56:13 2018 by ROOT version 6.10/08
// from TTree Readout/TriggerReadout
// found on file: Jinping_1ton_Phy_20171003_00000331.root
//////////////////////////////////////////////////////////

#ifndef Readout_h
#define Readout_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class Readout {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           RunNo;
   Int_t           TriggerNo;
   Int_t           TriggerType;
   Int_t           DetectorID;
   Int_t           Sec;
   Int_t           NanoSec;
   vector<unsigned int> *ChannelId;
   vector<unsigned int> *Waveform;

   // List of branches
   TBranch        *b_RunNo;   //!
   TBranch        *b_TriggerNo;   //!
   TBranch        *b_TriggerType;   //!
   TBranch        *b_DetectorID;   //!
   TBranch        *b_Sec;   //!
   TBranch        *b_NanoSec;   //!
   TBranch        *b_ChannelId;   //!
   TBranch        *b_Waveform;   //!

   Readout(TTree *tree=0);
   virtual ~Readout();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Readout_cxx
Readout::Readout(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Jinping_1ton_Phy_20171003_00000331.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("Jinping_1ton_Phy_20171003_00000331.root");
      }
      f->GetObject("Readout",tree);

   }
   Init(tree);
}

Readout::~Readout()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Readout::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Readout::LoadTree(Long64_t entry)
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

void Readout::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   ChannelId = 0;
   Waveform = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("RunNo", &RunNo, &b_RunNo);
   fChain->SetBranchAddress("TriggerNo", &TriggerNo, &b_TriggerNo);
   fChain->SetBranchAddress("TriggerType", &TriggerType, &b_TriggerType);
   fChain->SetBranchAddress("DetectorID", &DetectorID, &b_DetectorID);
   fChain->SetBranchAddress("Sec", &Sec, &b_Sec);
   fChain->SetBranchAddress("NanoSec", &NanoSec, &b_NanoSec);
   fChain->SetBranchAddress("ChannelId", &ChannelId, &b_ChannelId);
   fChain->SetBranchAddress("Waveform", &Waveform, &b_Waveform);
   Notify();
}

Bool_t Readout::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Readout::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Readout::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Readout_cxx
