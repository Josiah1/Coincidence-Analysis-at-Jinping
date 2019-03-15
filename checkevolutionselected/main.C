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
	
	TFile* f1=new TFile("375667_10day.root","RECREATE");
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/work/common-use-JSAP/JSAP/Analysis/TWin/Ostw/latestdata/";
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
        vector<Double_t> counter,counter_error;
        Int_t time_interval=60*60*24*10;//s
	//time of the first entry
        Double_t begin_time=0;
	//time of the last entry and break time between different run
        Double_t lasttimer=0,breaktime=0;
        //time of current entry
	Double_t currTime=0;
        //segment counter
	Int_t segi=0;
	//two event counters
        Int_t eventcounter=0;
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
		if(currTime<=(begin_time+(segi+1)*time_interval) && nEntries<entries){
                	//make your selection below
		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		Double_t time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
                Double_t x=(TR->E)[1];
                Double_t y=(TR->E)[0];

			if(TR->D2First[1] <=300 ){
                        	if(x>=40 && x<=80 && y>=80 && y<=220){
					if(time2first<=4.e-04)	{
                        			eventcounter+=1;
					}
				}
			}		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		}
		else {
			//record and restart
                        if(breaktime<time_interval/2. && breaktime>=0){
                                Double_t realtime=lasttimer-(begin_time+segi*time_interval)-breaktime;
				//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%				
				//set point time 
				time_b.push_back(begin_time+segi*time_interval);
                                time_b_error.push_back(0);
                                //set counter value
                                counter.push_back(eventcounter/realtime);
                                counter_error.push_back(TMath::Sqrt(eventcounter)/(realtime));

				//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        }
			breaktime=breaktimeinit;
                        //--------------
			eventcounter=0;
			//--------------
                         segi=segi+1+pop;
                	//make your selection below
			 
			Double_t time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
                	Double_t x=(TR->E)[1];
                	Double_t y=(TR->E)[0];
			if(TR->D2First[1] <=300 ){
			
                        	if(x>=40 && x<=80 && y>=80 && y<=220){
					if(time2first<=4.e-04)	{
                        			eventcounter+=1;
					}
				}
			}		

		}
	}
	
	TGraphErrors* gr1 = new TGraphErrors(time_b.size(), &(time_b[0]), &(counter[0]), &(time_b_error[0]), &(counter_error[0]));

      	//gr1->SetMarkerStyle(7);
        gr1->GetXaxis()->SetTimeDisplay(1);
        gr1->GetXaxis()->SetNdivisions(5);
        gr1->GetXaxis()->SetTimeFormat("%Y/%m/%d");
        gr1->GetXaxis()->SetTimeOffset(0,"gmt");
        gr1->GetXaxis()->SetLabelOffset(0.02);
        gr1->GetXaxis()->SetTitle("Date");
        gr1->GetYaxis()->SetTitle("Selected Bi-214 Event Rate(Hz)");
        gr1->GetYaxis()->SetTitleSize(0.05);
        gr1->SetTitle("Selected Bi-214 Event rate vs Date");
        gr1->SetMarkerColor(4);
	gr1->SetMarkerStyle(21);
	gr1->Write("gr1");
	
	//file close
	f1->Close();

	return 1;
}
