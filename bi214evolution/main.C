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
#include "TGraphAsymmErrors.h"
#include <vector>
#include "TMinuit.h"
#include "TStyle.h"

using namespace std;
Double_t mass=966643.2095;
Double_t tau=2.37271e-4;
Double_t lnLi(Double_t* x,Double_t* par){
        Double_t xx=x[0];
        Double_t f=par[0]*TMath::Exp(-xx/tau)/tau+par[1]*250;
        return f;
}

Double_t tpdf(Double_t* x,Double_t* par){
        Double_t xx=x[0];
        Double_t f=par[0]*TMath::Exp(-xx/tau)/tau+par[1]*250;
        return f/(par[0]+par[1]);
}
vector<double>* xVecPtr = new vector<double>();
void fcn(int& npar, double* deriv, double& f, double par[], int flag){

  vector<double> xVec = *xVecPtr;           // xVecPtr is global
  int n = xVec.size();

  double lnL = 0.0;
  for (int i=0; i<n; i++){
    double x = xVec[i];
    double pdf = tpdf(&x, par);
    double pdfi = lnLi(&x, par);
    if ( pdf > 0.0 ) {
      lnL += log(pdfi);    // need positive f
    }
    else {
      cout << "WARNING -- pdf is negative!!!" << endl;
    }
  }
  f = 2.0*(par[0]+par[1]-lnL);         // factor of -2 so minuit gets the errors right

  cout<<"par[0] "<<par[0]<<"\t"<<"par[1] "<<par[1]<<"\t"<<"-2lnL "<<f<<endl;

}                         // end of fcn

int main(int argc, char** argv)
{
	
	TFile* f1=new TFile("evolutioncheck.root","RECREATE");
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
	vector<Double_t> time_b,time_b_errorl,time_b_errorh;
        vector<Double_t> counter,counter_errorl,counter_errorh,sig,sig_errorl,sig_errorh;
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
	//Tminuit setup
	//Tminuit init
        const int npar = 2;              // the number of parameters
        TMinuit minuit(npar);
        minuit.SetFCN(fcn);

        double par[npar];               // the start values
        double stepSize[npar];          // step sizes 
        double minVal[npar];            // minimum bound on parameter 
        double maxVal[npar];            // maximum bound on parameter
        string parName[npar];

        par[0] = xVecPtr->size()/2.;            // a guess
        stepSize[0] = 0.001;       // take e.g. 0.1 of start value
        minVal[0] = 0.0000001;   // if min and max values = 0, parameter is unbounded.
        maxVal[0] = 100000000;
        parName[0] = "nsig";

        par[1] = xVecPtr->size()/2.;            // a guess
        stepSize[1] = 0.001;       // take e.g. 0.1 of start value
        minVal[1] = 0.0000001;   // if min and max values = 0, parameter is unbounded.
        maxVal[1] = 100000000;
        parName[1] = "nbkg";

        for (int i=0; i<npar; i++){
                minuit.DefineParameter(i, parName[i].c_str(),
                par[i], stepSize[i], minVal[i], maxVal[i]);
        }

        // Configure minuit
        minuit.SetErrorDef(1);             // delta -2lnL = 1 for Error definition
        minuit.SetMaxIterations(500);
        // BACKUP for data of point fitted! */

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
		 if((TR->QMax2Sum)[0]>0.3 || (TR->QMax2Sum)[1]>0.3) continue;
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		Double_t time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
                Double_t x=(TR->E)[1];
                Double_t y=(TR->E)[0];

			if(TR->D2First[1] <=300 ){
                        	if(x>=40 && x<=80 && y>=50 && y<=250){
					xVecPtr->push_back(time2first);
	
					if(time2first<=4.e-04)	{
                        			eventcounter+=1;
					}
				}
			}		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		}
		else {
			//record and restart
                        if(breaktime<time_interval && breaktime>=0){
				Double_t realtime=time_interval-breaktime;
                                if(nEntries==entries) realtime=currTime-(begin_time+segi*time_interval)-breaktime;
				
				//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%				
				par[0]=xVecPtr->size()/2.;
                                par[1]=xVecPtr->size()/2.;
                                minuit.DefineParameter(0, parName[0].c_str(), par[0], stepSize[0], minVal[0], maxVal[0]);
                                minuit.DefineParameter(1, parName[1].c_str(), par[1], stepSize[1], minVal[1], maxVal[1]);
                                minuit.Migrad();
                                double par,errl,errh,err;
                                double bkg,errb;
                                minuit.GetParameter(0,par,err);
                                minuit.GetParameter(1,bkg,errb);
                                if(par-err<=0) {
					
					par=TMath::Sqrt(eventcounter)*3+par;
					errl=TMath::Sqrt(eventcounter)*3;
					errh=0;
				}else
					{
						errh=err;
						errl=err;
					}
				//Fit result got
				//set point time 
				time_b.push_back(begin_time+segi*time_interval);
                                time_b_errorl.push_back(0);
                                time_b_errorh.push_back(0);
				//set signal value
                                sig.push_back(par/(realtime*mass));
                                sig_errorl.push_back(errl/(realtime*mass));
                                sig_errorh.push_back(errh/(realtime*mass));
                                //set counter value
                                counter.push_back(eventcounter/(realtime*mass));
                                counter_errorl.push_back(TMath::Sqrt(eventcounter)/(realtime*mass));
                                counter_errorh.push_back(TMath::Sqrt(eventcounter)/(realtime*mass));

				//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                        }
                        xVecPtr->clear(); 
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
			
                        	if(x>=40 && x<=80 && y>=50 && y<=250){
					xVecPtr->push_back(time2first);
	
					if(time2first<=4.e-04)	{
                        			eventcounter+=1;
					}
				}
			}		

		}
	}
	
	TGraphAsymmErrors* gr1 = new TGraphAsymmErrors(time_b.size(), &(time_b[0]), &(counter[0]), &(time_b_errorl[0]),&(time_b_errorh[0]), &(counter_errorl[0]),&(counter_errorh[0]));
	TGraphAsymmErrors* gr2 = new TGraphAsymmErrors(time_b.size(), &(time_b[0]), &(sig[0]), &(time_b_errorl[0]),&(time_b_errorh[0]), &(sig_errorl[0]),&(sig_errorh[0]));
	for(size_t i=0;i<time_b.size();i++){
		cout<<"Point: "<<i<<endl;
		cout<<"Time: "/*<<setiosflags(ios::fixed)<<setprecision(9)*/<<time_b[i]<<endl;
		cout<<"r_sig: "<<sig[i]<<endl;
		cout<<"r_sig h: "<<sig_errorl[i]<<endl;
		cout<<"r_sig l: "<<sig_errorh[i]<<endl;
	}
      	//gr1->SetMarkerStyle(7);
        gr1->GetXaxis()->SetTimeDisplay(1);
        gr1->GetXaxis()->SetNdivisions(3);
        gr1->GetXaxis()->SetTimeFormat("%Y/%m/%d");
        gr1->GetXaxis()->SetTimeOffset(0,"gmt");
        gr1->GetXaxis()->SetLabelOffset(0.02);
        gr1->GetXaxis()->SetTitle("Date");
        gr1->GetYaxis()->SetTitle("Selected Bi-214 Event Rate [Bq/g]");
        gr1->GetYaxis()->SetTitleSize(0.05);
        gr1->SetTitle("Selected Bi-214 Event rate vs Date");
        gr1->SetMarkerColor(4);
	gr1->SetMarkerStyle(21);
	gr1->Write("gr1");
	
        gr2->GetXaxis()->SetTimeDisplay(1);
        gr2->GetXaxis()->SetNdivisions(3);
        gr2->GetXaxis()->SetTimeFormat("%Y/%m/%d");
        gr2->GetXaxis()->SetTimeOffset(0,"gmt");
        gr2->GetXaxis()->SetLabelOffset(0.02);
        gr2->GetXaxis()->SetTitle("Date");
        gr2->GetYaxis()->SetTitle("Bi-214 Event Rate [Bq/g]");
        gr2->GetYaxis()->SetTitleSize(0.05);
        gr2->SetTitle("Fitted Bi-214 Event rate vs Date");
        gr2->SetMarkerColor(4);
	gr2->SetMarkerStyle(21);
	gr2->Write("gr2");

	//file close
	f1->Close();

	return 1;
}
