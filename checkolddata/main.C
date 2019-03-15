#include "Event.h"
#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TimeStamp.h"
#include "TCanvas.h"
using namespace std;

int main(int argc, char** argv)
{
	TString inputfiledir="/home/jinping/WORK/guozy/JinpingPackage/JinpingAnalysis/Analysis/TWin/Ostw/output/oldcalib/";
	TFile* f1=new TFile("oldcalib_coinc.root","RECREATE");
	TChain* tc = new TChain("Event");
	if(argc==3){
		for(Int_t i=atoi(argv[1]);i<=atoi(argv[2]);i++){
			tc->Add(TString::Format(inputfiledir+"Coinc_%d.root",i));
		}
	}else {cout<<"Usage:JPLooper run(begin,end)"<<endl;return 0;}
	// For root prompt run
	//ReadoutTreeReader* TR = (ReadoutTreeReader*)TSelector::GetSelector("ReadoutTreeReader.C");
	Event* TR = new Event;

	TR->Init(tc);

	TH2D* h1=new TH2D("h1","Prompt PE versus delayed PE",300,0,300,300,0,300);
        TH2D* h2=new TH2D("h2","Distance distribution",300,0,300,500,0,1000);
        TH1D* h3=new TH1D("h3","Distribution of delayed PE",300,0,300);
        TH1D* h4=new TH1D("h4","Distribution of Prompt PE",300,0,300);


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
                	if(TR->D2First[1] <=300 && time2first<=4.e-4 && *(TR->Run)<=290){
                        	h1->Fill(x,y);
				h3->Fill(x);
				h4->Fill(y);
                	}
                	if(time2first<=4.e-4 && *(TR->Run)<=290){
				h2->Fill(x,(TR->D2First)[1]);
                	}

        	}

	}
	h1->Write("h1");
	h2->Write("h2");
	h3->Write("h3");
	h4->Write("h4");
	cout<<"Read "<<nEntries<<" entries."<<endl;
	f1->Close();
	return 1;
}
