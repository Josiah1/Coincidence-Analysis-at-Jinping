#include "Event.h"
#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TimeStamp.h"
#include "TCanvas.h"
using namespace std;

int main(int argc, char** argv)
{
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/JinpingPackage/20180527/JSAP-install/Analysis/TWin/Ostw/output/";
	TFile* f1=new TFile("BetaandAlpha.root","RECREATE");
	TChain* tc = new TChain("Event");
	if(argc==3){
		for(Int_t i=atoi(argv[1]);i<=atoi(argv[2]);i++){
			tc->Add(TString::Format(inputfiledir+"%d.ostw.root",i));
		}
	}else {cout<<"Usage:JPLooper run(begin,end)"<<endl;return 0;}
	// For root prompt run
	//ReadoutTreeReader* TR = (ReadoutTreeReader*)TSelector::GetSelector("ReadoutTreeReader.C");
	Event* TR = new Event;

	TR->Init(tc);
	tc->LoadTree(0);
	
	TTree* BA=new TTree("BetaAlpha","Beta and Alpha Events");
	Int_t RunNo=0,TriggerNo_beta=0,TriggerNo_alpha=0,FileNum_beta=0,FileNum_alpha=0;
	BA->Branch("RunNo",&RunNo);
	BA->Branch("TriggerNo_beta",&TriggerNo_beta);
	BA->Branch("TriggerNo_alpha",&TriggerNo_alpha);
	BA->Branch("FileNum_beta",&FileNum_beta);
	BA->Branch("FileNum_alpha",&FileNum_alpha);
	Int_t nEntries = 0;
	tc->Write("Event");
	while(TR->fReader.Next())
	{
		nEntries++;
		if(*(TR->Fold) == 2){
                	Double_t time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
                	Double_t x=(TR->E)[1];
                	Double_t y=(TR->E)[0];
                	//distance cut begins
                	if((TR->QMax2Sum)[0]>0.3 || (TR->QMax2Sum)[1]>0.3) continue;
			if((TR->D2First)[1] <=300 && time2first<=4.e-4 && x>=40 && x<=80 && y>=50 && y<=250){
				RunNo=*(TR->Run);
				TriggerNo_beta=(TR->TrigNum)[0];
				TriggerNo_alpha=(TR->TrigNum)[1];
				FileNum_beta=(TR->FileNum)[0];
				FileNum_alpha=(TR->FileNum)[1];
				
				BA->Fill();
                	}
        	}

	}
	//--------------
	
	cout<<"Read "<<nEntries<<" entries."<<endl;
	BA->Write("BetaAlpha");
	f1->Close();
	
	return 1;
}
