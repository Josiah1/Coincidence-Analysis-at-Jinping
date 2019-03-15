#include <iostream>
#include <TMath.h>
Double_t f1(Double_t* x,Double_t* par){
	Double_t xx=x[0];
	
	Double_t ff=par[0]*TMath::Exp(-0.5*TMath::Power((xx-par[1])/par[2],2));
	return ff;
}
Double_t f2(Double_t* x,Double_t* par){
	Double_t xx=x[0];
	
	Double_t ff=par[0]*TMath::Exp(-0.5*TMath::Power((xx-par[1])/par[2],2));
	return ff;
}
