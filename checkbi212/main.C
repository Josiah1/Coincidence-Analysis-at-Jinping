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
	TFile* f1=new TFile("bi212_320667.root","RECREATE");
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
	TH2D* h1=new TH2D("h1","Prompt PE versus delayed PE",300,0,300,300,0,300);
        TH2D* h2=new TH2D("h2","Distance distribution",300,0,300,500,0,1000);
        TH1D* hd=new TH1D("hd","Distance distribution of Bi-214",500,0,800);
        TH1D* h3=new TH1D("h3","Distribution of delayed PE",300,0,300);
        TH1D* h4=new TH1D("h4","Distribution of Prompt PE",300,0,300);
        TH1D* ht=new TH1D("ht","Distribution of coincidence time",100,0,0.004);

	Int_t nEntries = 0;
	tc->Write("Event");
	while(TR->fReader.Next())
	{
		nEntries++;
		if(*(TR->Fold) == 2 && ((TMath::Abs(TR->Z[0])<=250 || TMath::Abs(TR->Z[1])<=250)
					&& (TMath::Abs(TR->Y[0])<=250 || TMath::Abs(TR->Y[1])<=250)
					&& (TMath::Abs(TR->X[0])<=250 || TMath::Abs(TR->X[1])<=250) ) ){
                	Double_t time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
                	Double_t x=(TR->E)[1];
                	Double_t y=(TR->E)[0];
                	//distance cut begins
                	if((TR->QMax2Sum)[0]>0.3 || (TR->QMax2Sum)[1]>0.3) continue;
			if((TR->D2First)[1] <=300 && time2first<=4.e-4){
                        	h1->Fill(x,y);
				h3->Fill(x);
				h4->Fill(y);
				
                	}
                	if(time2first<=4.e-4){
				h2->Fill(x,(TR->D2First)[1]);
                	}
			if(x>=40 && x<=80 && y>=50 && y<=250){
				if(time2first<=4.e-4) hd->Fill(TR->D2First[1]);
				if(TR->D2First[1]<=300) ht->Fill(time2first);
			}
        	}

	}
	h1->Write("h1");
	h2->Write("h2");
	h3->Write("h3");
	h4->Write("h4");
	hd->Write("hd");
	ht->Write("ht");
	//--------------
	//for life fit
	TCanvas* c1=new TCanvas();
	ht->Draw();
	TF1* fexp=new TF1("fexp","[0]+[1]*TMath::Exp(-x/[2])",4.e-5,4.e-3);
	fexp->SetParNames("A0","A1","#tau");
	fexp->SetParameters(1.e+03,2.5e+04,2.3e-04);
	ht->Fit("fexp","R");
	fexp->Write("fexp");
	c1->Write("fit");
	cout<<"Read "<<nEntries<<" entries."<<endl;
	f1->Close();
	return 1;
}
