#include "Event.h"
#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TH2.h"
#include "TimeStamp.h"
#include "TCanvas.h"
using namespace std;

int main(int argc, char** argv)
{
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/JinpingPackage/100tonSim_latest20180927_IBD_update2_samplepoint/JSAP-install/Analysis/TWin/Ostw/Bi214/";
	TFile* f1=new TFile("MC_coinc.root","RECREATE");
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
	h1->SetXTitle("Delayed Energy [PE]");
	h1->SetYTitle("Prompt Energy [PE]");
        TH2D* h2=new TH2D("h2","Distance distribution",300,0,300,500,0,1000);
	h2->SetXTitle("Delayed Energy [PE]");
	h2->SetYTitle("Distance [mm]");
        TH1D* hd=new TH1D("hd","Distance distribution of Bi-214",500,0,1000);
	hd->SetXTitle("Distance [mm]");
	hd->SetYTitle("Entries");
        TH1D* h3=new TH1D("h3","Distribution of delayed PE",100,0,100);
	h3->SetXTitle("Delayed Energy [PE]");
	h3->SetYTitle("Entries");
        TH1D* h4=new TH1D("h4","Distribution of Prompt PE",300,0,300);
	h4->SetXTitle("Prompt Energy [PE]");
	h4->SetYTitle("Entries");
        TH1D* ht=new TH1D("ht","Time between prompt and delayed signal",100,0,0.004);
	ht->SetXTitle("Time [s]");
	ht->SetYTitle("Entries");


	Int_t nEntries = 0;
	tc->Write("Event");
	Int_t numberbi214=0;
	while(TR->fReader.Next())
	{
		nEntries++;
		if(*(TR->Fold) == 2){
                	Double_t time2first=(TR->T2PrevSubEvt)[1];
                	Double_t x=(TR->E)[1];
                	Double_t y=(TR->E)[0];
			//if(TR->QMax2Sum[0]>0.3 || TR->QMax2Sum[1]>0.3) continue;
                	//distance cut begins
                	if((TR->D2First)[1] <=300 && time2first<=4.e-4){
                        	h1->Fill(x,y);
				h3->Fill(x);
				h4->Fill(y);
				
                	}
                	if(time2first<=4.e-4){
				h2->Fill(x,(TR->D2First)[1]);
                	}
			
			if(x>=40 && x<=80 && y>=50 && y<=250 && time2first<=4.e-4 ){
				hd->Fill(TR->D2First[1]);
			}
			if(x>=40 && x<=80 && y>=50 && y<=250 && TR->D2First[1]<=300 ){
				ht->Fill(time2first);
			}
        	}

	}
	h1->Write("h1");
	h2->Write("h2");
	h3->Write("h3");
	h4->Write("h4");
	hd->Write("hd");
	ht->Write("ht");

	TF1* fc=new TF1("fc","[0]+[1]*TMath::Exp(-x/[2])",4.e-5,4.e-3);
	fc->SetParNames("A0","A1","#tau");
	fc->SetParameters(100,5.e+04,2.3e-4);
	fc->Write("fc");
	cout<<"Read "<<nEntries<<" entries."<<endl;
	f1->Close();
	return 1;
}
