//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 30 22:18:37 2018 by ROOT version 6.10/08
// from TTree SimpleAnalysis/Analysis Stream Tree
// found on file: SimpleRecon_Analysis_20180527_1.root
//////////////////////////////////////////////////////////

#ifndef SimpleAnalysis_h
#define SimpleAnalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector


class SimpleAnalysis : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> RunNo = {fReader, "RunNo"};
   TTreeReaderValue<Int_t> FileNo = {fReader, "FileNo"};
   TTreeReaderValue<Int_t> TriggerNo = {fReader, "TriggerNo"};
   TTreeReaderValue<Int_t> TriggerType = {fReader, "TriggerType"};
   TTreeReaderValue<Int_t> DetectorID = {fReader, "DetectorID"};
   TTreeReaderValue<Int_t> Sec = {fReader, "Sec"};
   TTreeReaderValue<Int_t> NanoSec = {fReader, "NanoSec"};
   TTreeReaderValue<Int_t> nChannels = {fReader, "nChannels"};
   TTreeReaderArray<Int_t> ChannelId = {fReader, "ChannelId"};
   TTreeReaderArray<Double_t> PE = {fReader, "PE"};
   TTreeReaderArray<Double_t> Pedestal = {fReader, "Pedestal"};
   TTreeReaderArray<Double_t> Peak = {fReader, "Peak"};
   TTreeReaderArray<Double_t> RiseTime = {fReader, "RiseTime"};
   TTreeReaderArray<Double_t> FrontBslnMean = {fReader, "FrontBslnMean"};
   TTreeReaderArray<Double_t> FrontBslnStdDev = {fReader, "FrontBslnStdDev"};
   TTreeReaderArray<Double_t> BackBslnMean = {fReader, "BackBslnMean"};
   TTreeReaderArray<Double_t> BackBslnStdDev = {fReader, "BackBslnStdDev"};
   TTreeReaderValue<Int_t> nSaturatedChannels = {fReader, "nSaturatedChannels"};
   TTreeReaderValue<Double_t> TotalPE = {fReader, "TotalPE"};
   TTreeReaderValue<Double_t> Chi2 = {fReader, "Chi2"};
   TTreeReaderValue<Double_t> PEmax2Sum = {fReader, "PEmax2Sum"};
   TTreeReaderValue<Double_t> x = {fReader, "x"};
   TTreeReaderValue<Double_t> y = {fReader, "y"};
   TTreeReaderValue<Double_t> z = {fReader, "z"};
   TTreeReaderValue<Double_t> E = {fReader, "E"};


   SimpleAnalysis(TTree * /*tree*/ =0) { }
   virtual ~SimpleAnalysis() { }
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

   ClassDef(SimpleAnalysis,0);

};

#endif

#ifdef SimpleAnalysis_cxx
void SimpleAnalysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t SimpleAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef SimpleAnalysis_cxx
