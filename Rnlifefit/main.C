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
//global data
vector<double>* xVecPtr = new vector<double>();
Double_t tau=2.37271e-04;
//
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
//

Double_t fexp(Double_t *x,Double_t *par){
        Double_t xx=x[0];
        Double_t f=par[0]+par[1]*TMath::Exp(-xx/par[2]);
        return f;
}
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

} 
//for rate correction
double rate_b=12.090240;//Hz
double rate_c=23.663817;//Hz
double rate_d=27.358089;//Hz
Double_t base=0,B=0,C=0,D=0;
//for change from 1 day for one point to 10 day for one point
Int_t changepoint=0;
Int_t sys=0;
int main(int argc, char** argv)
{
	
	TFile* f1=new TFile("rn222.root","RECREATE");
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/JinpingPackage/20181108/JSAP-install/Analysis/TWin/Ostw/output/oneton/";
	TChain* tc = new TChain("Event");
	if(argc==3){
		for(Int_t i=atoi(argv[1]);i<=atoi(argv[2]);i++){
        		if(!(i>=474 && i<=481 && i>=487 && i<=488))
			tc->Add( TString::Format(inputfiledir+"%d.ostw.root",i));
		}
	}else {cout<<"Usage:JPLooper run(begin,end)"<<endl;return 0;}
	Int_t entries=tc->GetEntries("Fold==2");
	// For root prompt run
	//ReadoutTreeReader* TR = (ReadoutTreeReader*)TSelector::GetSelector("ReadoutTreeReader.C");
	Event* TR = new Event;

	TR->Init(tc);
	tc->LoadTree(0);
	//-------------------------------------------------------------------------------------------
	vector<Double_t> time_b,time_b_error,stno;
        vector<Double_t> counter,counter_error;
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
        Int_t eventcounter=0;
        //the previous RunNO
	Int_t preRunNo=atoi(argv[1]);
	//-------------------------------------------------------------------------------------------
	Double_t breaktimeinit=0;
	Int_t nEntries = 0;

	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
	//-----------------------
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
			Double_t upperboundtime=begin_time+(segi-changepoint+1)*time_interval;
                        if(currTime<=upperboundtime) {
				breaktime+=currTime-lasttimer;
			}
                        if(currTime>upperboundtime) {
				Double_t afterbreak=currTime-upperboundtime;
				pop=floor(afterbreak/time_interval);
				
				breaktime+=upperboundtime-lasttimer;
				breaktimeinit=currTime-(upperboundtime+pop*time_interval);
			}
                }
		//record last runno
                preRunNo=*(TR->Run);
	
		if((TR->QMax2Sum)[0]>0.3 || (TR->QMax2Sum)[1]>0.3) continue;
		if(currTime<=(begin_time+(segi+1-changepoint)*time_interval) && nEntries<entries){
                	//make your selection below
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		Double_t time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
                Double_t x=(TR->E)[1];
                Double_t y=(TR->E)[0];

			if(TR->D2First[1] <=300 ){
				//get time data
                        	if(x>=40 && x<=80 && y>=50 && y<=250 /*&& time2first<=4.e-4*/){
                        		xVecPtr->push_back(time2first);
					base+=1;
					if(*(TR->Run)>=291 && *(TR->Run)<=296) B+=1;	
					if(*(TR->Run)>=297 && *(TR->Run)<=319) C+=1;	
					if(*(TR->Run)>=320) D+=1;	
				}
			}		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		}
		else {
			//record and restart
                        if(breaktime<time_interval/2. && breaktime>=0){
				//calculating real data time
                                Double_t realtime=time_interval-breaktime;
				if(nEntries==entries) realtime=currTime-(begin_time+(segi-changepoint)*time_interval)-breaktime;
				//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
				par[0]=xVecPtr->size()/2.;
                                par[1]=xVecPtr->size()/2.;
                                minuit.DefineParameter(0, parName[0].c_str(), par[0], stepSize[0], minVal[0], maxVal[0]);
                                minuit.DefineParameter(1, parName[1].c_str(), par[1], stepSize[1], minVal[1], maxVal[1]);
                                minuit.Migrad();
                                double par1,err1;
                                double par2,err2;
                                minuit.GetParameter(0,par1,err1);
                                minuit.GetParameter(1,par2,err2);
				//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
				//rate correction: scale event rate to phase D
				par1=par1*(1+B/base*(rate_d/rate_b-1)+C/base*(rate_d/rate_c-1));
				//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
				time_b.push_back(begin_time+(segi-changepoint)*time_interval);
                                stno.push_back(segi);
                                time_b_error.push_back(0);
                                counter.push_back(par1/realtime);
                                counter_error.push_back(err1/(realtime));
                                //cout<<"One point got!!! :"<<" segi: "<<segi<<" eventcounter: "<<eventcounter<<" currTime: "<<currTime<<" lasttimer: "<<lasttimer<<" breaktime: "<<breaktime<<endl;
                        }
			//cout<<"refresh-breaktimeinit: "<<breaktimeinit<<endl;
                        breaktime=breaktimeinit;
			xVecPtr->clear();			
			base=0,B=0,C=0,D=0;
                        //--------------
			eventcounter=0;
                	//make your selection below
			 
			Double_t time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
                	Double_t x=(TR->E)[1];
                	Double_t y=(TR->E)[0];
			if(TR->D2First[1] <=300 ){
                        	if(x>=40 && x<=80 && y>=50 && y<=250){
                        		xVecPtr->push_back(time2first);
					base+=1;
					if(*(TR->Run)>=291 && *(TR->Run)<=296) B+=1;	
					if(*(TR->Run)>=297 && *(TR->Run)<=319) C+=1;	
					if(*(TR->Run)>=320) D+=1;	
				}
			}
			//set 10 day for one point to fit dut to low rate 		
			if(*(TR->Run)>=291) {
				if(sys==0) {
					begin_time=currTime;
					changepoint=segi;
				}
				sys++;
				time_interval=60*60*24*10;//s
			}
			
			//--------------
                        segi=segi+1+pop;
		}
	}
	for(size_t i=0;i<stno.size();i++){
		if(stno[i]>(changepoint+1)) stno[i]=changepoint+1+(stno[i]-changepoint-1)*10;
	}
	TGraphErrors* gr1 = new TGraphErrors(time_b.size(), &(time_b[0]), &(counter[0]), &(time_b_error[0]), &(counter_error[0]));
	TGraphErrors* gr2 = new TGraphErrors(stno.size(), &(stno[0]), &(counter[0]), &(time_b_error[0]), &(counter_error[0]));
	TCanvas* clog=new TCanvas();
	gr2->GetXaxis()->SetTitle("Day");
	gr2->GetYaxis()->SetTitle("Bi-214 Event Rate(Hz)");
        gr2->SetMarkerStyle(7);
        gr2->SetMarkerColor(4);
	clog->SetLogy();
	gr2->Draw("AP");
	gr2->Write("gr2");
	clog->SaveAs("log.pdf");
	clog->Close();

        gr1->SetMarkerStyle(7);
        gr1->GetXaxis()->SetTimeDisplay(1);
        gr1->GetXaxis()->SetNdivisions(5);
        gr1->GetXaxis()->SetTimeFormat("%Y/%m/%d");
        gr1->GetXaxis()->SetTimeOffset(0,"gmt");
        gr1->GetXaxis()->SetLabelOffset(0.02);
        gr1->GetXaxis()->SetTitle("Date");
        gr1->GetYaxis()->SetTitle("Bi-214 Event Rate(Hz)");
        gr1->GetYaxis()->SetTitleSize(0.05);
        gr1->SetTitle("Bi-214 Event rate vs Date");
        gr1->SetMarkerColor(4);
	//gr1->SetMarkerStyle(21);
        TCanvas* cgr1=new TCanvas();
	gr1->Draw("AP");
	gr1->Write("gr1");
	cgr1->SaveAs("gr1.pdf");
	
	
	gStyle->SetOptFit(1);	
	TF1* ff=new TF1("ff",fexp,0,30,3);
	ff->SetParNames("A0","A1","#tau");
	ff->SetParameters(2.e-04,0.2,5.5);
	gr2->Fit("ff","R");
	ff->Write("ff");
	cgr1->SaveAs("rn222life.pdf");
	f1->Close();

	return 1;
}
