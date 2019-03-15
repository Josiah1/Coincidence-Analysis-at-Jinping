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
#include "TMath.h"
#include "TGraphErrors.h"
#include <vector>
#include "TMinuit.h"
#include "TStyle.h"

using namespace std;

int main(int argc, char** argv)
{
	
	TFile* f1=new TFile("CoinRate.root","RECREATE");
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/JinpingPackage/20181108/JSAP-install/Analysis/TWin/Ostw/output/oneton/";
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
	vector<Double_t> time_b,time_b_error;
        vector<Double_t> counter,counter_error,pecounter,pecounter_error,pedcounter,pedcounter_error,pedtcounter,pedtcounter_error;
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
        Int_t eventcounter=0,peeventcounter,pedeventcounter,pedteventcounter;
        //the previous RunNO
	Int_t preRunNo=atoi(argv[1]);
	//-------------------------------------------------------------------------------------------
	Double_t breaktimeinit=0;

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	Int_t nEntries = 0;
	while(TR->fReader.Next() )
	{
		if(*(TR->Fold)!=2) continue;
		if(nEntries==0){
			begin_time=TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0]).GetSeconds();
			lasttimer=begin_time;
			currTime=begin_time;
		}	
		nEntries++;
		//cout<<"read: "<<nEntries<<endl;
		breaktimeinit=0;
		//record last trigger time
		lasttimer=currTime;
		//record this trigger time
                currTime=TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0]).GetSeconds();
		//run break point
		Int_t pop=0;
		if(preRunNo!=*(TR->Run)) {
                        if(currTime<=begin_time+(segi+1)*time_interval) {
				breaktime+=currTime-lasttimer;
			}
                        if(currTime>begin_time+(segi+1)*time_interval ) {
				Double_t afterbreak=currTime-(begin_time+(segi+1)*time_interval);
				pop=floor(afterbreak/time_interval);
				
				breaktime+=begin_time+(segi+1)*time_interval-lasttimer;
				breaktimeinit=currTime- (begin_time+(segi+1+pop)*time_interval);
			}
                }
		//record last runno
                preRunNo=*(TR->Run);
		if((TR->QMax2Sum)[0]>0.3 || (TR->QMax2Sum)[1]>0.3) continue;
		if(currTime<=(begin_time+(segi+1)*time_interval) && nEntries<entries){
                	//make your selection below
		
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                 	eventcounter+=1;
			if((TR->E)[0]>=50 && (TR->E)[0]<=250 && (TR->E)[1]<=80 && (TR->E)[1]>=40){
				peeventcounter+=1;
				if((TR->D2First)[1]<=300){
					pedeventcounter+=1;
					Double_t time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
					if(time2first<=4.e-4) pedteventcounter+=1;
				}
			}
			//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		}
		else {
			//record and restart
                        if(breaktime<time_interval/2. && breaktime>=0){
				Double_t realtime=time_interval-breaktime;
                                if(nEntries==entries) realtime=currTime-(begin_time+segi*time_interval)-breaktime;
				
				//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%				
				//set point time 
				time_b.push_back(begin_time+segi*time_interval);
                                time_b_error.push_back(0);
                                //set counter value
                                counter.push_back(eventcounter/realtime);
                                counter_error.push_back(TMath::Sqrt(eventcounter)/(realtime));

                                pecounter.push_back(peeventcounter/realtime);
                                pecounter_error.push_back(TMath::Sqrt(peeventcounter)/(realtime));
                                
				pedcounter.push_back(pedeventcounter/realtime);
                                pedcounter_error.push_back(TMath::Sqrt(pedeventcounter)/(realtime));
				pedtcounter.push_back(pedteventcounter/realtime);
                                pedtcounter_error.push_back(TMath::Sqrt(pedteventcounter)/(realtime));
				//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        }
			breaktime=breaktimeinit;
                        //--------------
			eventcounter=0;
			peeventcounter=0;
			pedeventcounter=0;
			pedteventcounter=0;
			//--------------
                         segi=segi+1+pop;
                	//make your selection below
                        eventcounter+=1;
		}
	}
	
	TGraphErrors* gr1 = new TGraphErrors(time_b.size(), &(time_b[0]), &(counter[0]), &(time_b_error[0]), &(counter_error[0]));
	TGraphErrors* gr2 = new TGraphErrors(time_b.size(), &(time_b[0]), &(pecounter[0]), &(time_b_error[0]), &(pecounter_error[0]));
	TGraphErrors* gr3 = new TGraphErrors(time_b.size(), &(time_b[0]), &(pedcounter[0]), &(time_b_error[0]), &(pedcounter_error[0]));
	TGraphErrors* gr4 = new TGraphErrors(time_b.size(), &(time_b[0]), &(pedtcounter[0]), &(time_b_error[0]), &(pedtcounter_error[0]));

      	//gr1->SetMarkerStyle(7);
        gr1->GetXaxis()->SetTimeDisplay(1);
        gr1->GetXaxis()->SetNdivisions(5);
        gr1->GetXaxis()->SetTimeFormat("%Y/%m/%d");
        gr1->GetXaxis()->SetTimeOffset(0,"gmt");
        gr1->GetXaxis()->SetLabelOffset(0.02);
        gr1->GetXaxis()->SetTitle("Date");
        gr1->GetYaxis()->SetTitle("2-fold coincidental Event Rate [Hz]");
        gr1->GetYaxis()->SetTitleSize(0.05);
        gr1->SetTitle(" ");
        gr1->SetMarkerColor(4);
	gr1->SetMarkerStyle(21);
	gr1->Write("gr1");
	
        gr2->GetXaxis()->SetTimeDisplay(1);
        gr2->GetXaxis()->SetNdivisions(5);
        gr2->GetXaxis()->SetTimeFormat("%Y/%m/%d");
        gr2->GetXaxis()->SetTimeOffset(0,"gmt");
        gr2->GetXaxis()->SetLabelOffset(0.02);
        gr2->GetXaxis()->SetTitle("Date");
        gr2->GetYaxis()->SetTitle("2-fold coincidental Event( after PE cut) Rate [Hz]");
        gr2->GetYaxis()->SetTitleSize(0.05);
        gr2->SetTitle(" ");
        gr2->SetMarkerColor(4);
	gr2->SetMarkerStyle(21);
	gr2->Write("gr2");
        
	gr3->GetXaxis()->SetTimeDisplay(1);
        gr3->GetXaxis()->SetNdivisions(5);
        gr3->GetXaxis()->SetTimeFormat("%Y/%m/%d");
        gr3->GetXaxis()->SetTimeOffset(0,"gmt");
        gr3->GetXaxis()->SetLabelOffset(0.02);
        gr3->GetXaxis()->SetTitle("Date");
        gr3->GetYaxis()->SetTitle("2-fold coincidental Event( after PE and distance cut) Rate [Hz]");
        gr3->GetYaxis()->SetTitleSize(0.05);
        gr3->SetTitle(" ");
        gr3->SetMarkerColor(4);
	gr3->SetMarkerStyle(21);
	gr3->Write("gr3");
        
	gr4->GetXaxis()->SetTimeDisplay(1);
        gr4->GetXaxis()->SetNdivisions(5);
        gr4->GetXaxis()->SetTimeFormat("%Y/%m/%d");
        gr4->GetXaxis()->SetTimeOffset(0,"gmt");
        gr4->GetXaxis()->SetLabelOffset(0.02);
        gr4->GetXaxis()->SetTitle("Date");
        gr4->GetYaxis()->SetTitle("2-fold coincidental Event( after PE, distance and time cut) Rate [Hz]");
        gr4->GetYaxis()->SetTitleSize(0.05);
        gr4->SetTitle(" ");
        gr4->SetMarkerColor(4);
	gr4->SetMarkerStyle(21);
	gr4->Write("gr4");
	//file close
	f1->Close();

	return 1;
}
