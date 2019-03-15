#include "Event.h"
#include <iostream>
#include <string>
#include "TChain.h"
#include "TimeStamp.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "f.C"
#include "TString.h"
#include "TMath.h"
using namespace std;

int main(int argc,char** argv)
{
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/work/common-use-JSAP/JSAP/Analysis/TWin/Ostw/ostwoutput/";
        TString outdir;
        Int_t begin,end;
        TChain *tc = new TChain("Event");

        if( argc != 4 ) {
                cout<<"Usage:"<<endl;
                cout<<"    ./accCONFIR inputfile(begin,end) outputdir/"<<endl;
                cout<<endl;
                return 1;
        } else {
                begin = atoi(argv[1]);
                end = atoi(argv[2]);
                outdir=argv[3];
                for(Int_t i=begin;i<=end;i++){
                        tc->Add( TString::Format(inputfiledir+"%d.ostw.root",i));
                }
        }


	// For root prompt run
	//ReadoutTreeReader* TR = (ReadoutTreeReader*)TSelector::GetSelector("ReadoutTreeReader.C");
	Event* TR = new Event;
	
	TR->Init(tc);
	gStyle->SetOptFit(1);
	TH1D* h1=new TH1D("h1","prompt PE",300,0,300);
	TH1D* h2=new TH1D("h2","delayed PE",300,0,300);
	
	TF1* tf1=new TF1("tf1",f1,40,140,3);
	TF1* tf11=new TF1("tf11",f1,140,300,3);
	TF1* tf1t=new TF1("tf1t","gaus(0)+gaus(3)",40,300);
	
	TF1* tf2=new TF1("tf2",f2,40,140,3);
	TF1* tf22=new TF1("tf22",f2,140,300,3);
	TF1* tf2t=new TF1("tf2t","gaus(0)+gaus(3)",40,300);
	
	tf1t->SetParNames("A1","#mu_1","#sigma_1","A2","#mu_2","#sigma_2");
	tf2t->SetParNames("A1","#mu_1","#sigma_1","A2","#mu_2","#sigma_2");
	tf1->SetParameters(4.e+05,110,10);
	tf11->SetParameters(2.e+05,180,10);
	tf2->SetParameters(4.e+05,110,10);
	tf22->SetParameters(2.e+05,180,10);
	Int_t nEntries = tc->GetEntries();
	//while(!TR->fReader.SetEntry(nEntries))
	for(Int_t i=0;i<nEntries;i++)
	{
		TR->GetEntry(i);
		if(int(TR->Fold) == 2){
                        Double_t time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
                        Double_t x=(TR->E)[1];
                        Double_t y=(TR->E)[0];
			
			h1->Fill(y);
			h2->Fill(x);
                }

		
	}
	Double_t par1[6];
	Double_t par2[6];
	h1->Fit("tf1","LRQNO");
	h1->Fit("tf11","LRQNO+");
	h2->Fit("tf2","LRQN0");
	h2->Fit("tf22","LRQN0+");
	tf1->GetParameters(&par1[0]);
	tf11->GetParameters(&par1[3]);
	tf2->GetParameters(&par2[0]);
	tf22->GetParameters(&par2[3]);
	tf1t->SetParameters(par1);
	tf2t->SetParameters(par2);

	TCanvas* c1=new TCanvas();
	h1->GetXaxis()->SetTitle("PE");
	h1->Draw();
	h1->Fit("tf1","LR+");
	c1->SaveAs(outdir+"h1_sp.pdf");
	c1->Close();
	TCanvas* c2=new TCanvas();
	h2->GetXaxis()->SetTitle("PE");
	h2->Draw();
	h2->Fit("tf2","LR+");
	c2->SaveAs(outdir+"h2_sp.pdf");
	c2->Close();
		

	TFile* file=new TFile(outdir+"ll.root","RECREATE");
	tf1->Write("tf1");
	tf2->Write("tf2");
	h1->Write("h1");
	h2->Write("h2");
	file->Close();
	cout<<"Read "<<nEntries<<" entries."<<endl;

}
