#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TF1.h"
#include "TH1.h"
#include "TimeStamp.h"
#include "TTree.h"
#include "TMath.h"
#include <vector>
#include <map>
#include "TSystem.h"

using namespace std;


int main(int argc, char** argv)
{
	
	TFile* f1=new TFile("datatime_320910.root","RECREATE");
	TTree* datatimetree=new TTree("timelist","data time of every single run");
	Double_t time=0,sum=0;
	Int_t run=0;
	datatimetree->Branch("RunNo",run);
	datatimetree->Branch("RunTime",time);
	
	Int_t c=0;
	TString inputfiledir="/back41/jinping_back4/data/Jinping_1ton_Data/01_RawData";
	TChain* tc[1000];
	map<Int_t,Double_t> datatimelist;
	if(argc==3){
		Int_t begin=atoi(argv[1]);
		Int_t end=atoi(argv[2]);
		for(Int_t i=begin;i<=end;i++){
			//bad data is discard
        		if(!((i>=474 && i<=481) || (i>=487 && i<=488) || (i<555 && i>=529))){ 
			if(!gSystem->AccessPathName(TString::Format(inputfiledir+"/run%08d/Jinping_1ton_Phy_*_%08d.root", i, i))) continue;
			cout<<"Add: "<<i<<endl;
			tc[c]=new TChain("Readout");
			tc[c]->Add(TString::Format(inputfiledir+"/run%08d/Jinping_1ton_Phy_*_%08d.root", i, i));
    			
			Int_t fileno = 1;
   			 while(true)
    			{
        			TString localFilename = TString::Format(inputfiledir+"/run%08d/Jinping_1ton_Phy_*_%08d_%d.root", i, i, fileno);
        			if(tc[c]->Add(localFilename)==0)
        			{
            				//cout<<localFilename<<endl;
            				break;
        			}
        			fileno++;
    			}
			//single run file add ok
			Int_t entries=tc[c]->GetEntries();
			Int_t Sec=0.,NanoSec=0.;
			tc[c]->SetBranchAddress("Sec",&Sec);
			tc[c]->SetBranchAddress("NanoSec",&NanoSec);
			tc[c]->GetEntry(0);
			Double_t sec1=Sec,nano1=NanoSec;	
			tc[c]->GetEntry(entries-1);
			Double_t sec2=Sec,nano2=NanoSec;	
			run=i;
			c++;

			time=(TimeStamp(sec2,nano2)-TimeStamp(sec1,nano1)).GetSeconds();
			datatimelist[i]=time;
			datatimetree->Fill();
			sum+=time;
			cout<<"Run: "<<run<<" runtime: "<<setiosflags(ios::fixed)<<setprecision(8)<<time<<endl;
			}
		}

	}else {cout<<"Usage:JPLooper run(begin,end)"<<endl;return 0;}
	
	cout<<"Total dada time: "<<setiosflags(ios::fixed)<<setprecision(8)<<sum<<endl;
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	
	datatimetree->Write();
	

	//file close
	f1->Close();

	return 1;
}
