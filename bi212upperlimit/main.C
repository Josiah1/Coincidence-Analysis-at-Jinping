#include "Event.h"
#include <iostream>
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TF1.h"
#include "TH1.h"
#include "TimeStamp.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include <vector>
#include "TMinuit.h"
#include "TStyle.h"
#include "TLegend.h"

using namespace std;

double tau_sig=4.328085123e-07;
double databegin=1.e-06,dataend=5.e-06;
double uniformConstant=1/(dataend-databegin);

Double_t fexp(Double_t* x,Double_t* par){
        Double_t xx=x[0];
        Double_t tau=par[2];
	double expuniformConstant=TMath::Exp(-databegin/tau)-TMath::Exp(-dataend/tau);
        Double_t f=par[1]/expuniformConstant*TMath::Exp(-xx/tau)/tau+par[0];
        return f;
}
Double_t lnLi(Double_t* x,Double_t* par){
        Double_t xx=x[0];
        double expuniformConstant=TMath::Exp(-databegin/tau_sig)-TMath::Exp(-dataend/tau_sig);
        Double_t f=par[0]/expuniformConstant*TMath::Exp(-xx/tau_sig)/tau_sig+par[1]*uniformConstant;
        return f;
}

Double_t tpdf(Double_t* x,Double_t* par){
        Double_t xx=x[0];
        double expuniformConstant=TMath::Exp(-databegin/tau_sig)-TMath::Exp(-dataend/tau_sig);
        Double_t f=par[0]/expuniformConstant*TMath::Exp(-xx/tau_sig)/tau_sig+par[1]*uniformConstant;
        return f/(par[0]+par[1]);
}
vector<double>* xVecPtr = new vector<double>();
void fcn(int& npar, double* deriv, double& f, double par[], int flag){

  vector<double> xVec = *xVecPtr;           // xVecPtr is global
  int n = xVec.size();

  double lnL = 0.0;
  for (int i=0; i<n; i++){
    double x = xVec[i];//for specical use; For normal use please change to:  double x = xVec[i];
    if(!(x>=databegin && x<=dataend)) continue;
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
	
	//TFile* f1=new TFile("result_timecorrection.root","READ");
	TFile* file2=new TFile("result_v20180626.root","RECREATE");
	TTree* t1=new TTree("DataTree","Data for calculating UpperLimit");
	t1->Branch("data",xVecPtr);	

	//TTree* t1=(TTree*)f1->Get("DataTree");
	//t1->SetBranchAddress("data",&xVecPtr);	
	//t1->GetEntry(0);
	
	//TString inputfiledir="/work/wangzhe10_work/JinjingLi/JinpingPackage/20180527/JSAP-install/Analysis/TWin/Ostw/output/";
	TString inputfiledir="/work/wangzhe10_work/JinjingLi/work/common-use-JSAP/JSAP/Analysis/TWin/Ostw/timecorrection/";
	TChain* tc = new TChain("Event");
	if(argc==3){
		cout<<"File aldready input."<<endl;
		for(Int_t i=atoi(argv[1]);i<=atoi(argv[2]);i++){
			//bad data is discard
        		if(!((i>=474 && i<=481) || (i>=487 && i<=488) || (i<555 && i>=529))) tc->Add( TString::Format(inputfiledir+"%d.ostw.root",i));
		}
	}else {cout<<"Usage:JPLooper run(begin,end)"<<endl;return 0;}
	//Int_t entries=tc->GetEntries("Fold==2");
	// For root prompt run
	//ReadoutTreeReader* TR = (ReadoutTreeReader*)TSelector::GetSelector("ReadoutTreeReader.C");
	Event* TR = new Event;

	TR->Init(tc);
	tc->LoadTree(0);
	//-------------------------------------------------------------------------------------------
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

        par[0] = 100;            // a guess
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
		nEntries++;
		//cout<<"read: "<<nEntries<<endl;
		if((TR->QMax2Sum)[0]>0.3 || (TR->QMax2Sum)[1]>0.3) continue;
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		Double_t time2first=(TimeStamp((TR->TrigSec)[1],(TR->TrigNano)[1])-TimeStamp((TR->TrigSec)[0],(TR->TrigNano)[0])).GetSeconds();
                Double_t x=(TR->E)[1];
                Double_t y=(TR->E)[0];

			if(TR->D2First[1] <=400 ){
                        	if(x>=50 && x<=100 && y>=50 && y<=250 && time2first<=25.e-6 && time2first>=21.e-6){
					xVecPtr->push_back(time2first-20.e-6);
	
				}
			}		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	}
	t1->Fill();
	t1->Write();
	
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%				
        minuit.DefineParameter(0, parName[0].c_str(), par[0], stepSize[0], minVal[0], maxVal[0]);
        minuit.DefineParameter(1, parName[1].c_str(), par[1], stepSize[1], minVal[1], maxVal[1]);
        minuit.Migrad();
        double par1,err1;
        double par2,err2;
        minuit.GetParameter(0,par1,err1);
        minuit.GetParameter(1,par2,err2);
        
	Double_t amin,edm,errdef;
        Int_t nvpar,nparx,icstat;
        minuit.mnstat(amin,edm,errdef,nvpar,nparx,icstat);
	
	TF1* fbest=new TF1("fbest",lnLi,databegin,dataend,2);
	Int_t bin=50;
	fbest->SetParameters(par1*(dataend-databegin)/(double)bin,par2*(dataend-databegin)/(double)bin);
	TH1D* hdata=new TH1D("hdata","",bin,databegin,dataend);
	for(size_t i=0;i<xVecPtr->size();i++){
		hdata->Fill((*xVecPtr)[i]);
	}
	//Fit achieved, now find upper limit
	cout<<"Begin upper limit check."<<endl;
        Double_t ssize=0.2;
        Double_t hatamin=0;
        vector<double> q,nsig_scan;
        for(Int_t i=0.;i<=1000;i++){
                
                par[0]=i*ssize;
                nsig_scan.push_back(par[0]);
                minuit.DefineParameter(0, parName[0].c_str(), par[0], stepSize[0], minVal[0], maxVal[0]);
                minuit.FixParameter(0);
                minuit.Migrad();
                minuit.mnstat(hatamin,edm,errdef,nvpar,nparx,icstat);
                q.push_back( hatamin-amin );
        }
        

        cout<<"Event number: "<<xVecPtr->size()<<endl;

        for(size_t i=0;i<q.size();i++){
                cout<<"q: "<<nsig_scan[i]<<" = "<<q[i]<<endl;
        }

	//show the q versus nsig
	TLegend* leg = new TLegend();
	TGraph* gr1=new TGraph(q.size(),&(nsig_scan[0]),&(q[0]));
	TF1* ff1=new TF1("ff1","2.71",0,200);
	leg->AddEntry(ff1,"q=2.71 corresponding to 90%C.L.","l");
	leg->AddEntry(gr1,"q versus #nu-signal","l");
	TCanvas* c1=new TCanvas();	
	gr1->Draw();
	ff1->Draw("same");
	leg->Draw("same");
	c1->SaveAs("qn.pdf");
	c1->Close();
	//f1->Close();

	gr1->Write("gr1");
	ff1->Write("f1");
	c1->Write("qn");
	
	fbest->Write("fbest");
	hdata->Write("hdata");
	file2->Close();
	//-------------------------------------------------------------------
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//file close

	return 1;
}
