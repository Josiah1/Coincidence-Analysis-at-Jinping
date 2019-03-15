#include "Event.h"
#include <iostream>
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TimeStamp.h"
using namespace std;


int main(int argc, char** argv)
{

  TString inputfiledir="/work/wangzhe10_work/JinjingLi/work/common-use-JSAP/JSAP/Analysis/TWin/Ostw/ostwoutput/";
  TChain *tc = new TChain("Event");

  if( argc != 3 ) {
    cout<<"Usage:"<<endl;
    cout<<"    Looper Run(begin,end)"<<endl;
    cout<<endl;
    return 1;
  } else {
    Int_t a=atoi(argv[1]);
    Int_t b=atoi(argv[2]);

    for(Int_t i=a;i<=b;i++)
        tc->Add( TString::Format(inputfiledir+"%d.ostw.root",i));
  }

	// For root prompt run
	//ReadoutTreeReader* TR = (ReadoutTreeReader*)TSelector::GetSelector("ReadoutTreeReader.C");
	auto* TR = new Event;

	TR->Init(tc);
	Int_t           b_Run;
   	Int_t           b_Det;
   	Double_t        b_T2PrevMu;
   	Double_t        b_T2PrevPlMu;
   	Double_t        b_T2PrevAdMu;
   	Double_t        b_T2PrevShMu;
   	Double_t        b_T2PrevNetMu;
   	Int_t           b_Fold;
   	Int_t           b_FileNum[b_Fold];   //[Fold]
   	Int_t           b_TrigNum[b_Fold];   //[Fold]
   	Int_t           b_TrigSec[b_Fold];   //[Fold]
   	Int_t           b_TrigNano[b_Fold];   //[Fold]
   	Double_t        b_dis[b_Fold];   //[Fold]
   	Double_t        b_E[b_Fold];   //[Fold]
   	Double_t        b_X[b_Fold];   //[Fold]
   	Double_t        b_Y[b_Fold];   //[Fold]
   	Double_t        b_Z[b_Fold];   //[Fold]
   	Double_t        b_D2First[b_Fold];   //[Fold]
   	Double_t        b_T2PrevSubEvt[b_Fold];   //[Fold]
   	Double_t        b_Quadrant[b_Fold];   //[Fold]
   	Double_t        b_QMax2Sum[b_Fold];   //[Fold]
	Double_t 	b_time2first;

	TFile* file=new TFile("coincidence_old.root","RECREATE");
	TTree* ft=new TTree("Event","Coincidence Event");
	ft->Branch("Run", &b_Run);
   	ft->Branch("Det", &b_Det);
   	ft->Branch("T2PrevMu", &b_T2PrevMu);
   	ft->Branch("T2PrevPlMu", &b_T2PrevPlMu);
   	ft->Branch("T2PrevAdMu", &b_T2PrevAdMu);
   	ft->Branch("T2PrevShMu", &b_T2PrevShMu);
   	ft->Branch("T2PrevNetMu", &b_T2PrevNetMu);
   	ft->Branch("Fold", &b_Fold);
   	ft->Branch("FileNum", b_FileNum);
  	ft->Branch("TrigNum", b_TrigNum);
   	ft->Branch("TrigSec", b_TrigSec);
   	ft->Branch("TrigNano", b_TrigNano);
   	ft->Branch("dis", b_dis);
   	ft->Branch("E", b_E);
   	ft->Branch("X", b_X);
   	ft->Branch("Y", b_Y);
   	ft->Branch("Z", b_Z);
   	ft->Branch("D2First", b_D2First);
   	ft->Branch("T2PrevSubEvt", b_T2PrevSubEvt);
   	ft->Branch("Quadrant", b_Quadrant);
   	ft->Branch("QMax2Sum", b_QMax2Sum);
   	ft->Branch("time2first", &b_time2first);


	Int_t nEntries = tc->GetEntries();
	//while(!TR->fReader.SetEntry(nEntries))
	for(Int_t j=0;j<nEntries;j++)
	{
		TR->GetEntry(j);
		if(TR->Fold==2){
			b_time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
		
		
			b_Run=TR->Run;
        		b_Det=TR->Det;
                	b_T2PrevMu=TR->T2PrevMu;
                	b_T2PrevPlMu=TR->T2PrevPlMu;
                	b_T2PrevAdMu=TR->T2PrevAdMu;
                	b_T2PrevShMu=TR->T2PrevShMu;
                	b_T2PrevNetMu=TR->T2PrevNetMu;
                	b_Fold=TR->Fold;
			for(Int_t i=0;i<b_Fold;i++){
				b_FileNum[i]=(TR->FileNum)[i];   //[i]
        			b_TrigNum[i]=(TR->TrigNum)[i];   //[i]
        			b_TrigSec[i]=(TR->TrigSec)[i];   //[i]
                        	b_TrigNano[i]=(TR->TrigNano)[i];   //[i]
                 		b_dis[i]=(TR->dis)[i];   //[i]
                 		b_E[i]=(TR->E)[i];   //[i]
                 		b_X[i]=(TR->X)[i];   //[i]
                 		b_Y[i]=(TR->Y)[i];   //[i]
                 		b_Z[i]=(TR->Z)[i];   //[i]
                 		b_D2First[i]=(TR->D2First)[i];   //[i]
                 		b_T2PrevSubEvt[i]=(TR->T2PrevSubEvt)[i];   //[i]
                 		b_Quadrant[i]=(TR->Quadrant)[i];   //[i]
                 		b_QMax2Sum[i]=(TR->QMax2Sum)[i]; 
			}	
			ft->Fill();
		}
	}
	ft->Write();
	file->Close();
	cout<<"Read "<<nEntries<<" entries."<<endl;

}
