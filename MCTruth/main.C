#include "SimpleAnalysis.h"
#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TF1.h"
#include "TMath.h"
#include "TSystem.h"
#include "TH2.h"
#include "TimeStamp.h"
#include "TCanvas.h"
#include "JPSimOutput.hh"
using namespace std;

int main(int argc, char** argv)
{
	gSystem->Load(gSystem->ExpandPathName("$JSAPSYS/DataType/lib/libJPSIMOUTPUT.so"));
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/JinpingPackage/20180527/JSAP-install/Simulation/output/";
	TFile* f1=new TFile("MC_truth.root","RECREATE");
	TChain* tc = new TChain("SimTriggerInfo");
	if(argc==3){
		for(Int_t i=atoi(argv[1]);i<=atoi(argv[2]);i++){
			tc->Add(TString::Format(inputfiledir+"20180527_%d.root",i));
			Int_t fileno = 1;
    			while(true)
    			{
				TString localFilename = TString::Format(inputfiledir+"20180527_%d_%d.root", i, fileno);
        			if(!gSystem->AccessPathName(localFilename))
        				tc->Add(localFilename);
				else break;
        			fileno++;
    			}
		}
	}else {cout<<"Usage:JPLooper run(begin,end)"<<endl;return 0;}
	// For root prompt run
	//ReadoutTreeReader* TR = (ReadoutTreeReader*)TSelector::GetSelector("ReadoutTreeReader.C");
	Int_t pdgID=0;
	std::vector<JPSimTruthTree_t>* truthList = new std::vector<JPSimTruthTree_t>;
	tc->SetBranchAddress("truthList", &truthList);
	Int_t nEntries = tc->GetEntries();
	Int_t p=0,d=0;
	for(Int_t i=0;i<nEntries;i++)
	{
		tc->GetEntry(i);
			for(auto&& itemj : (*truthList)){
				for(auto&& item : itemj.PdgId)
				{
					//find alpha
					if(item==1000020040) d+=1;
					//find beta
					if(item==11) p+=1;
				}
			}

	}
	cout<<"p: "<<p<<" ;d: "<<d<<endl;
	cout<<"Read "<<nEntries<<" entries."<<endl;
	f1->Close();
	return 1;
}
