#include "SimpleAnalysis.h"
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
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/JinpingPackage/latest/JSAP/Reconstruction/SimpleRecon/quenched/";
	TFile* f1=new TFile("MC_coinc.root","RECREATE");
	TChain* tc = new TChain("SimpleAnalysis");
	if(argc==3){
		for(Int_t i=atoi(argv[1]);i<=atoi(argv[2]);i++){
			tc->Add(TString::Format(inputfiledir+"SimpleRecon_Analysis_20180527_%d.root",i));
		}
	}else {cout<<"Usage:JPLooper run(begin,end)"<<endl;return 0;}
	// For root prompt run
	//ReadoutTreeReader* TR = (ReadoutTreeReader*)TSelector::GetSelector("ReadoutTreeReader.C");
	SimpleAnalysis* TR = new SimpleAnalysis;

	TR->Init(tc);
	tc->LoadTree(0);
        TH1D* h=new TH1D("h","Distribution PE",300,0,300);
	Int_t nEntries = 0;
	tc->Write("SimpleAnalysis");
	Int_t p=0,d=0;
	while(TR->fReader.Next())
	{
		nEntries++;
		//if(*(TR->PEmax2Sum)>0.3) continue;
		Bool_t bValid = kTRUE;
        	/*for(Int_t j=0; j<*(TR->nChannels); j++)
        	{
                	if(  TR->FrontBslnStdDev[j]>3)
               	 	{
                        	bValid = kFALSE;
                        	break;
                	}
		
        	}*/
		if(bValid) {
			Double_t PE=*(TR->TotalPE);
			h->Fill(PE);
			if(PE>=40 && PE<=80) d+=1;
			if(PE>=50 && PE<=250) p+=1;
		}	

	}
	h->Write("h");

	cout<<"Read "<<nEntries<<" entries."<<endl;
	cout<<"beta: "<<p<<" ;alpha: "<<d<<endl;
	f1->Close();
	return 1;
}
