#include "SimpleAnalysis.h"
#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TimeStamp.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include <vector>
using namespace std;

int main(int argc, char** argv)
{
	
	TFile* f1=new TFile("eventrate_1hour.root","RECREATE");
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/JinpingPackage/latest/JSAP/Analysis/PreAnalysis/inner/v1.2data/";
	TChain* tc = new TChain("SimpleAnalysis");
	if(argc==3){
		for(Int_t i=atoi(argv[1]);i<=atoi(argv[2]);i++){
        		if(!(i>=474 && i<=481 && i>=487 && i<=488))
			tc->Add( TString::Format(inputfiledir+"Analysis_%d.root",i));
		}
	}else {cout<<"Usage:JPLooper run(begin,end)"<<endl;return 0;}
	Int_t entries=tc->GetEntries();
	// For root prompt run
	//ReadoutTreeReader* TR = (ReadoutTreeReader*)TSelector::GetSelector("ReadoutTreeReader.C");
	SimpleAnalysis* TR = new SimpleAnalysis;

	TR->Init(tc);
	tc->LoadTree(0);
	//-------------------------------------------------------------------------------------------
	vector<Double_t> time_b,time_b_error;
        vector<Double_t> counter,counter_error,cut_counter,cut_counter_error;
        Int_t time_interval=60*60*24;//s
	//time of the first entry
        Double_t begin_time=0;
	//time of the last entry and break time between different run
        Double_t lasttimer=0,breaktime=0;
        //time of current entry
	Double_t currTime=0;
        //segment counter
	Int_t segi=0;
	//two event counters
        Int_t eventcounter=0,cut_eventcounter=0;
        //the previous RunNO
	Int_t preRunNo=atoi(argv[1]);
	//-------------------------------------------------------------------------------------------
	Double_t breaktimeinit=0;
	Int_t nEntries = 0;
	while(TR->fReader.Next())
	{
	
		if(nEntries==0){
			begin_time=TimeStamp(*(TR->Sec),*(TR->NanoSec)).GetSeconds();
			lasttimer=begin_time;
			currTime=begin_time;
		}	
		nEntries++;
		//cout<<"read: "<<nEntries<<endl;
		breaktimeinit=0;
		//record last trigger time
		lasttimer=currTime;
		//record this trigger time
                currTime=TimeStamp(*(TR->Sec),*(TR->NanoSec)).GetSeconds();
		//run break point
		Int_t pop=0;
		if(preRunNo!=*(TR->RunNo)) {
                        if(currTime<=begin_time+(segi+1)*time_interval) {
				breaktime+=currTime-lasttimer;
				//cout<<"inner break: "<<breaktime<<" from "<<preRunNo<<" to "<<*(TR->RunNo)<<endl;
			}
                        if(currTime>begin_time+(segi+1)*time_interval ) {
				Double_t afterbreak=currTime-(begin_time+(segi+1)*time_interval);
				Int_t breakno=int(afterbreak/time_interval);
		
				pop=floor(breakno);
				breaktime+=begin_time+(segi+1)*time_interval-lasttimer;
				breaktimeinit=currTime- (begin_time+(segi+1+pop)*time_interval);
				//cout<<"outter break: "<<breaktime<<" from "<<preRunNo<<" to "<<*(TR->RunNo)<<" the left init: "<<breaktimeinit<<endl;
			}
                        //cout<<"old run: "<<preRunNo<<" ;this run: "<<TR->Run<<" break time: "<<currTime-lasttimer<<endl;
                }
		//record last runno
                preRunNo=*(TR->RunNo);
		if(currTime<=(begin_time+(segi+1)*time_interval) && nEntries<entries){
                        //if(eventcounter %10000==0)cout<<"segi: "<<segi<<" eventcounter: "<<eventcounter<<" currTime: "<<currTime<<" bound time: "<<begin_time+(segi+1)*time_interval<<endl;
			eventcounter+=1;
			if(*(TR->PEmax2Sum)<=0.3 && *(TR->TotalPEd)>=40 && *(TR->TotalPEd)<=80 && *(TR->TotalPEp)>=50 && *(TR->TotalPEp)<=250 && *(TR->pddistance)<=300) cut_eventcounter+=1;
                }
		else {
			//record and restart
                        if(breaktime<time_interval/2. && breaktime>=0){
                                Double_t realtime=lasttimer-(begin_time+segi*time_interval)-breaktime;
				
				time_b.push_back(begin_time+segi*time_interval);
                                //time_b.push_back(segi);
                                time_b_error.push_back(0);
                                counter.push_back(eventcounter/realtime);
                                counter_error.push_back(TMath::Sqrt(eventcounter)/realtime);
                                cut_counter.push_back(cut_eventcounter/realtime);
                                cut_counter_error.push_back(TMath::Sqrt(cut_eventcounter)/realtime);
                                //cout<<"One point got!!! :"<<" segi: "<<segi<<" eventcounter: "<<eventcounter<<" currTime: "<<currTime<<" lasttimer: "<<lasttimer<<" breaktime: "<<breaktime<<endl;
                        }
			//cout<<"refresh-breaktimeinit: "<<breaktimeinit<<endl;
                        breaktime=breaktimeinit;
                        //--------------
			eventcounter=0;
                        cut_eventcounter=0;
			//record this event
			eventcounter+=1;
			if(*(TR->PEmax2Sum)<=0.3 && *(TR->TotalPEd)>=40 && *(TR->TotalPEd)<=80 && *(TR->TotalPEp)>=50 && *(TR->TotalPEp)<=250 && *(TR->pddistance)<=300) cut_eventcounter+=1;
			//--------------
                        segi=segi+1+pop;
		}
	}
	TGraphErrors* gr1 = new TGraphErrors(time_b.size(), &(time_b[0]), &(counter[0]), &(time_b_error[0]), &(counter_error[0]));
	TGraphErrors* gr2 = new TGraphErrors(time_b.size(), &(time_b[0]), &(cut_counter[0]), &(time_b_error[0]), &(cut_counter_error[0]));

        //TGraph* gr = new TGraph(time_b.size(), &(time_b[0]), &(counter[0]));
        gr1->SetMarkerStyle(7);
        gr1->GetXaxis()->SetTimeDisplay(1);
        gr1->GetXaxis()->SetNdivisions(5);
        gr1->GetXaxis()->SetTimeFormat("%Y/%m/%d");
        gr1->GetXaxis()->SetTimeOffset(0,"gmt");
        gr1->GetXaxis()->SetLabelOffset(0.02);
        gr1->GetXaxis()->SetTitle("Date");
        gr1->GetYaxis()->SetTitle("Event Rate(HZ)");
        gr1->GetYaxis()->SetTitleSize(0.05);
        gr1->SetTitle("Event rate vs Date");
        gr1->SetMarkerColor(4);
	//gr1->SetMarkerStyle(21);
        TCanvas* cgr1=new TCanvas();
	gr1->Draw("AP");
	gr1->Write("gr1");
	cgr1->SaveAs("gr1.pdf");
	cgr1->Close();

        gr2->GetXaxis()->SetTimeDisplay(1);
        gr2->GetXaxis()->SetNdivisions(5);
        gr2->GetXaxis()->SetTimeFormat("%Y/%m/%d");
        gr2->GetXaxis()->SetTimeOffset(0,"gmt");
        gr2->GetXaxis()->SetLabelOffset(0.02);
        gr2->GetXaxis()->SetTitle("Date");
        gr2->GetYaxis()->SetTitle("Event Rate(HZ)");
        gr2->GetYaxis()->SetTitleSize(0.05);
        gr2->SetTitle("Event rate vs Date");
        gr2->SetMarkerColor(4);
	gr2->SetMarkerStyle(7);
        TCanvas* cgr2=new TCanvas();
	gr2->Draw("AP");
	gr2->Write("gr2");
	cgr2->SaveAs("gr2.pdf");
	cgr2->Close();
	
	f1->Close();

	return 1;
}
