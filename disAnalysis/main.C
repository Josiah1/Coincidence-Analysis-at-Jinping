#include "Event.h"
#include <iostream>
#include <string>
#include "TChain.h"
#include "TH2.h"
#include "TFile.h"
using namespace std;

int main(int argc,char** argv)
{
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/work/common-use-JSAP/JSAP/Analysis/TWin/Ostw/timecorrection/";
        TString outdir;
        Int_t begin,end;
	TFile* global_file=new TFile("result.root","RECREATE");
        TChain *tc = new TChain("Event");

        if( argc != 4 ) {
                cout<<"Usage:"<<endl;
                cout<<"    ./JP inputfile(begin,end) outputdir"<<endl;
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

	TH2D* h1=new TH2D("h1","",200,0,0.5,200,0,300);
	TH2D* h2=new TH2D("h2","",200,0,0.5,200,0,300);
	TH2D* h3=new TH2D("h3","",200,0,0.5,200,0,300);
	TH2D* h4=new TH2D("h4","",200,0,0.5,200,0,300);
	TH2D* h5=new TH2D("h5","",200,0,0.5,200,0,300);
	TH2D* h6=new TH2D("h6","",200,0,0.5,200,0,300);
	
	TH2D* h7=new TH2D("h7","",200,0,300,200,0,300);
	TH2D* h8=new TH2D("h8","",200,0,300,200,0,300);
	TH1D* hp=new TH1D("hp","",200,0,0.5);
	TH1D* hd=new TH1D("hd","",200,0,0.5);
	TH1D* ht=new TH1D("ht","",200,0,0.5);
	// For root prompt run
	//ReadoutTreeReader* TR = (ReadoutTreeReader*)TSelector::GetSelector("ReadoutTreeReader.C");
	Event* TR = new Event;

	TR->Init(tc);
	tc->LoadTree(0);
	Int_t nEntries = 0;
	//while(!TR->fReader.SetEntry(nEntries))
	while(TR->fReader.Next())
	{
		ht->Fill(TR->dis[0]);
		ht->Fill(TR->dis[1]);
		if(*(TR->Fold)==2){
			Double_t x=TR->E[1],y=TR->E[0];
			
			h1->Fill(TR->dis[0],y);
			h2->Fill(TR->dis[1],x);
			h7->Fill(x,y);
			if(TR->D2First[1]<=300 && TR->T2PrevSubEvt[1]<=4.e-4){
				h3->Fill(TR->dis[0],y);
				h4->Fill(TR->dis[1],x);
				h8->Fill(x,y);
				if(x>=40 && x<=80) {
					h5->Fill(TR->dis[0],y);
					hp->Fill(TR->dis[0]);
				}
				if(y>=50 && y<=250) {
					h6->Fill(TR->dis[1],x);
					hd->Fill(TR->dis[0]);
				}
				
			}
		}
		


		nEntries++;
	}
	h1->Write("h1");
	h2->Write("h2");
	h3->Write("h3");
	h4->Write("h4");
	h5->Write("h5");
	h6->Write("h6");
	h7->Write("h7");
	h8->Write("h8");

	hp->Scale(1/(double)hp->GetEntries());
	hd->Scale(1/(double)hd->GetEntries());
	ht->Scale(1/(double)ht->GetEntries());
	hp->Write("hp");
	hd->Write("hd");
	ht->Write("ht");
	cout<<"Read "<<nEntries<<" entries."<<endl;
	global_file->Close();
	}
