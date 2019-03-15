//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 30 13:57:40 2018 by ROOT version 6.10/08
// from TTree Event/Analysis Tree
// found on file: 331.ostw.root
//////////////////////////////////////////////////////////

#ifndef Event_h
#define Event_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector


class Event : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> Run = {fReader, "Run"};
   TTreeReaderValue<Int_t> Det = {fReader, "Det"};
   TTreeReaderValue<Double_t> T2PrevMu = {fReader, "T2PrevMu"};
   TTreeReaderValue<Double_t> T2PrevPlMu = {fReader, "T2PrevPlMu"};
   TTreeReaderValue<Double_t> T2PrevAdMu = {fReader, "T2PrevAdMu"};
   TTreeReaderValue<Double_t> T2PrevShMu = {fReader, "T2PrevShMu"};
   TTreeReaderValue<Double_t> T2PrevNetMu = {fReader, "T2PrevNetMu"};
   TTreeReaderValue<Int_t> Fold = {fReader, "Fold"};
   TTreeReaderArray<Int_t> FileNum = {fReader, "FileNum"};
   TTreeReaderArray<Int_t> TrigNum = {fReader, "TrigNum"};
   TTreeReaderArray<Int_t> TrigSec = {fReader, "TrigSec"};
   TTreeReaderArray<Int_t> TrigNano = {fReader, "TrigNano"};
   TTreeReaderArray<Double_t> E = {fReader, "E"};
   TTreeReaderArray<Double_t> X = {fReader, "X"};
   TTreeReaderArray<Double_t> Y = {fReader, "Y"};
   TTreeReaderArray<Double_t> Z = {fReader, "Z"};
   TTreeReaderArray<Double_t> D2First = {fReader, "D2First"};
   TTreeReaderArray<Double_t> T2PrevSubEvt = {fReader, "T2PrevSubEvt"};
   TTreeReaderArray<Double_t> Quadrant = {fReader, "Quadrant"};
   TTreeReaderArray<Double_t> QMax2Sum = {fReader, "QMax2Sum"};


   Event(TTree * /*tree*/ =0) { }
   virtual ~Event() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(Event,0);

};

#endif

#ifdef Event_cxx
void Event::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
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


#endif // #ifdef Event_cxx
