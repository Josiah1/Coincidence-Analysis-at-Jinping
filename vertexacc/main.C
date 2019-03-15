#include "Event.h"
#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH2.h"
#include "TF1.h"
#include "TH1.h"
#include "TimeStamp.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TTree.h"
#include <vector>
#include "TLegend.h"

using namespace std;

int main(int argc, char** argv)
{
	
	TFile* f1=new TFile("vertex_acc.root","RECREATE");
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/JinpingPackage/20180527/JSAP-install/Analysis/TWin/Ostw/output/";
	TChain* tc = new TChain("Event");
	if(argc==3){
		for(Int_t i=atoi(argv[1]);i<=atoi(argv[2]);i++){
			//bad data is discard
        		if(!((i>=474 && i<=481) || (i>=487 && i<=488) || (i<555 && i>=529))) tc->Add( TString::Format(inputfiledir+"%d.ostw.root",i));
		}
	}else {cout<<"Usage:JPLooper run(begin,end)"<<endl;return 0;}
	Int_t entries=tc->GetEntries("Fold==2");
	// For root prompt run
	//ReadoutTreeReader* TR = (ReadoutTreeReader*)TSelector::GetSelector("ReadoutTreeReader.C");
	Event* TR = new Event;

	TR->Init(tc);
	tc->LoadTree(0);
	//-------------------------------------------------------------------------------------------
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        vector<Double_t> zp,rp,zd,rd,rhop,rhod;
	// BACKUP for data of point fitted! */
	TTree* datatree=new TTree("vertex","vertex of acc");
        datatree->Branch("zp",&zp);
        datatree->Branch("zd",&zd);
        datatree->Branch("rp",&rp);
        datatree->Branch("rd",&rd);
        datatree->Branch("rhop",&rhop);
        datatree->Branch("rhod",&rhod);
        
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	Int_t nEntries = 0;
	while(TR->fReader.Next() )
	{
		if(*(TR->Fold)!=2) continue;
		nEntries++;
		//cout<<"read: "<<nEntries<<endl;
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		Double_t time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
                Double_t x=(TR->E)[1];
                Double_t y=(TR->E)[0];
                        	if(true){
					cout<<nEntries<<endl;
					zp.push_back((TR->Z)[0]);
                                	zd.push_back((TR->Z)[1]);
                               	 	rp.push_back((TR->X)[0]*(TR->X)[0]+(TR->Y)[0]*(TR->Y)[0]);
                                	rd.push_back((TR->X)[1]*(TR->X)[1]+(TR->Y)[1]*(TR->Y)[1]);
                                	rhop.push_back(TMath::Power((TR->X)[0]*(TR->X)[0]+(TR->Y)[0]*(TR->Y)[0]+(TR->Z)[0]*(TR->Z)[0],1.5));
                                	rhod.push_back(TMath::Power((TR->X)[1]*(TR->X)[1]+(TR->Y)[1]*(TR->Y)[1]+(TR->Z)[1]*(TR->Z)[1],1.5));
				}
					
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	}
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%				
	
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	datatree->Fill();
	
	datatree->Write("vertex");
	//----------------------------------------
        TH2D* hp=new TH2D("hp"," ",200,0,360000,200,-800,800);
        TH2D* hd=new TH2D("hd"," ",200,0,360000,200,-800,800);
        for(size_t i=0;i<zp.size();i++){
                hp->Fill(rp[i],zp[i]);
                hd->Fill(rd[i],zd[i]);
        }
        hp->GetXaxis()->SetTitle("x^{2}+y^{2}[mm^{2}]");
        hp->GetYaxis()->SetTitle("z(mm)");
        hd->GetXaxis()->SetTitle("x^{2}+y^{2}[mm^{2}]");
        hd->GetYaxis()->SetTitle("z(mm)");
        TCanvas* c1=new TCanvas();
        c1->Divide(2,1);
        c1->cd(1);
        hp->Draw("COLZ");
        c1->cd(2);
        hd->Draw("COLZ");
        c1->SaveAs("vertex_cy_acc.pdf");

        TH1D* hprho=new TH1D("hprho"," ",60,0,3.e+08);
        TH1D* hdrho=new TH1D("hdrho"," ",60,0,3.e+08);
        for(size_t i=0;i<rhop.size();i++){
                hprho->Fill(rhop[i]);
                hdrho->Fill(rhod[i]);
        }
        hprho->GetXaxis()->SetTitle("r^{3}[mm^{3}]");
        hdrho->GetXaxis()->SetTitle("r^{3}[mm^{3}]");
        TCanvas* c2=new TCanvas();
        c2->Divide(2,1);
        c2->cd(1);
        hprho->Draw();
        c2->cd(2);
        hdrho->Draw();
        c2->SaveAs("vertex_sp_acc.pdf");

	hp->Write();
	hd->Write();
	hprho->Write();
	hdrho->Write();
	//file close
	f1->Close();
	cout<<"nEntries: "<<nEntries<<endl;
	return 1;
}
