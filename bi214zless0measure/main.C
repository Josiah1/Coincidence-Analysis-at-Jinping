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

Double_t tau0=2.37271e-4;


Double_t fexp(Double_t* x,Double_t* par){
        Double_t xx=x[0];
        Double_t tau=par[2];
        Double_t f=par[1]*TMath::Exp(-xx/tau)/tau+par[0];
        return f;
}
Double_t lnLi(Double_t* x,Double_t* par){
        Double_t xx=x[0];
        Double_t f=par[0]*TMath::Exp(-xx/tau0)/tau0+par[1]*250;
        return f;
}

Double_t tpdf(Double_t* x,Double_t* par){
        Double_t xx=x[0];
        Double_t f=par[0]*TMath::Exp(-xx/tau0)/tau0+par[1]*250;
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
	
	TFile* f1=new TFile("contamination_cutfores.root","RECREATE");
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
        vector<Double_t> maptime;
        TF1* fitf=new TF1("fitf",lnLi,0,4.e-03,2);
	TTree* fittimetree=new TTree("timedata","data");
        fittimetree->Branch("maptime",&maptime);


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

			if(TR->D2First[1] <=300 ){
                        	//if(x>=40 && x<=80 && y>=50 && y<=250){
                        	if(x>=40 && x<=80 && y>=50 && y<=250 && (TR->Z[0]<=0 || TR->Z[1]<=0)){
					xVecPtr->push_back(time2first);
                                        maptime.push_back(time2first);
	
				}
			}		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	}
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%				
	par[0]=xVecPtr->size()/2.;
        par[1]=xVecPtr->size()/2.;
        minuit.DefineParameter(0, parName[0].c_str(), par[0], stepSize[0], minVal[0], maxVal[0]);
        minuit.DefineParameter(1, parName[1].c_str(), par[1], stepSize[1], minVal[1], maxVal[1]);
        minuit.Migrad();
        double par1,err1;
        double par2,err2;
        minuit.GetParameter(0,par1,err1);
        minuit.GetParameter(1,par2,err2);
        
	Int_t bin=100;
	Double_t scal=0.004/(bin*1.0);
        //set function patameters
	fitf->SetParameters(par1*scal,par2*scal);
	//Fit achieved, now show it
	TLegend* leg = new TLegend();
	TH1D* h1=new TH1D("h1","Time between Bi214 prompt and delayed signal",bin,0,0.004);
	for(size_t i=0;i<xVecPtr->size();i++){
		h1->Fill(xVecPtr->at(i));
	}
	h1->SetXTitle("Time[s]");
	leg->AddEntry(fitf,"Function from unbinned likelihood fit","l");
	leg->AddEntry(h1,"Data from 2017-09-27 to 2018-07-30","l");
	TCanvas* c1=new TCanvas();	
	h1->Draw();
	fitf->Draw("same");
	leg->Draw("same");
	h1->Write("hdata");
	fitf->Write("f_unbinned");
	c1->Write("UnbinnedShow");
	c1->SaveAs("UnbinnedShow.pdf");
	c1->Close();
	//-------------------------------------------------------------------
	//Add binned data fit
	gStyle->SetOptFit(1);
	TF1* fb=new TF1("fb",fexp,4.e-05,0.004,3);
	fb->SetParNames("A0","A1","#tau");
	fb->SetParameters(1.6e+03,8.8e+02,2.1e+04);
	
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	fittimetree->Fill();
        fb->Write();
	
	fittimetree->Write("fittime");
	

	//file close
	f1->Close();

	return 1;
}
