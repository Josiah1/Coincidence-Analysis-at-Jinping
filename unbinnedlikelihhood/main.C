#include "Event.h"
#include <iostream>
#include "TChain.h"
#include "TTree.h"
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
#include "TRandom3.h"
#include "TSystem.h"

using namespace std;
double databegin=290,dataend=700;
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
        Double_t tau_sig=par[2];
	double expuniformConstant=TMath::Exp(-databegin/tau_sig)-TMath::Exp(-dataend/tau_sig);
        Double_t f=par[0]/expuniformConstant*TMath::Exp(-xx/tau_sig)/tau_sig+par[1]*uniformConstant;
        return f;
}

Double_t tpdf(Double_t* x,Double_t* par){
        Double_t xx=x[0];
        Double_t tau_sig=par[2];
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

  cout<<"par[0] "<<par[0]<<"\t"<<"par[1] "<<par[1]<<"\t"<<"par[2] "<<par[2]<<"\t"<<"-2lnL "<<f<<endl;

}                         // end of fcn

int main(int argc, char** argv)
{
	//TString file="/work/wangzhe10_work/JinjingLi/JinpingPackage/latest/JSAP/Analysis/PreAnalysis/innerMCbi212/allbi212.root";
	TString file="/work/wangzhe10_work/JinjingLi/JinpingPackage/latest/JSAP/Analysis/PreAnalysis/inner/v1.1data/";
	TFile* f1=new TFile(file,"READ");
	TFile* file2=new TFile("result.root","RECREATE");
	
	Double_t pdtime;
	Double_t pddistance,ppe,dpe,PEmax2Sum;
	//TTree* t1=(TTree*)f1->Get("SimpleAnalysis");
	TChain* t1=new TChain("SimpleAnalysis");
	for(Int_t i=257;i<=499;i++){
		TString localfile=TString::Format(file+"Analysis_%d.root",i);
		if(!gSystem->AccessPathName(localfile)) t1->Add(localfile);
	}
	t1->SetBranchAddress("pdtime", &pdtime);	
	t1->SetBranchAddress("pddistance", &pddistance);	
	t1->SetBranchAddress("TotalPEp", &ppe);	
	t1->SetBranchAddress("TotalPEd", &dpe);	
	t1->SetBranchAddress("PEmax2Sum", &PEmax2Sum);	
	
	//t1->GetEntry(0);
	cout<<"data points: "<<t1->GetEntries()<<endl;
	for(Int_t i=0;i<t1->GetEntries();i++){
		t1->GetEntry(i);
		if(pddistance<=300 && ppe>=50 && ppe<=250 && dpe>=40 && dpe<=80 && PEmax2Sum<=0.3)xVecPtr->push_back(-pdtime);
	}
	//-------------------------------------------------------------------------------------------
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//Tminuit setup
	//Tminuit init
        const int npar = 3;              // the number of parameters
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

        par[2] = 432.8;            // a guess
        stepSize[2] = 0.001;       // take e.g. 0.1 of start value
        minVal[2] = 0.0000001;   // if min and max values = 0, parameter is unbounded.
        maxVal[2] = 100000000;
        parName[2] = "#tau";

        for (int i=0; i<npar; i++){
                minuit.DefineParameter(i, parName[i].c_str(),
                par[i], stepSize[i], minVal[i], maxVal[i]);
        }
	//minuit.FixParameter(2);
	//minuit.FixParameter(0);
	//minuit.FixParameter(1);
        // Configure minuit
        minuit.SetErrorDef(1);             // delta -2lnL = 1 for Error definition
        minuit.SetMaxIterations(500);
        // BACKUP for data of point fitted! */
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%				
        for(size_t i=0;i<npar;i++){
		minuit.DefineParameter(i, parName[i].c_str(), par[i], stepSize[i], minVal[i], maxVal[i]);
	}
        minuit.Migrad();
        double parf[npar],errf[npar];
        for(size_t i=0;i<npar;i++){
        	minuit.GetParameter(i,parf[i],errf[i]);
        }
	Double_t amin,edm,errdef;
        Int_t nvpar,nparx,icstat;
        minuit.mnstat(amin,edm,errdef,nvpar,nparx,icstat);
	
	TF1* fbest=new TF1("fbest",lnLi,databegin,dataend,npar);
	Int_t bin=50;
	fbest->SetParameter(0,parf[0]*(dataend-databegin)/(bin*1.0));
	fbest->SetParameter(1,parf[1]*(dataend-databegin)/(bin*1.0));
	fbest->SetParameter(2,parf[2]);
	TH1D* hdata=new TH1D("hdata","",bin,databegin,dataend);
	for(size_t i=0;i<xVecPtr->size();i++){
		hdata->Fill((*xVecPtr)[i]);
	}
	//show the result
	TLegend* leg = new TLegend();
	leg->AddEntry(fbest,"Best fit from unbinned likelihood fit","l");
	leg->AddEntry(hdata,"Data","l");
	TCanvas* c1=new TCanvas();	
	hdata->Draw();
	fbest->Draw("same");
	leg->Draw("same");
	c1->SaveAs("fit.pdf");
	f1->Close();

	fbest->Write("fbest");
	hdata->Write("hdata");
	c1->Write("fit");
	
	file2->Close();
	//-------------------------------------------------------------------
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	//file close

	return 1;
}
