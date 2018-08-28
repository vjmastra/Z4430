#include <TStyle.h>
#include <TGaxis.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TROOT.h>

//gInterpreter->AddIncludePath("/cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/roofit/5.34.09-cms5/include") ;
//gROOT->ProcessLine(".include /cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/roofit/5.34.09-cms5/include") ;
// one of the 2 above lines need to be executed interactively before following includes:
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooConstVar.h>
#include <RooChebychev.h>
#include <RooAddPdf.h>
#include <RooAbsReal.h>
#include <RooPlot.h>
#include <RooCBShape.h>
#include <RooHist.h>

#include "RooJohnsonSU.h"

using namespace RooFit; // needed even without .C+

// lines above needed with .C+
/*
gSystem->Load("libRooFit");
using namespace RooFit; // needed even without .C+
*/

Float_t B0mass = 5.27961;

void b_peakFit() {

  //gROOT->Execute(".L RooJohnsonSU.cxx+","");
  gSystem->CompileMacro("RooJohnsonSU.cxx", "kO-");

  gStyle->SetOptStat(0);
  //gStyle->SetTitleOffset(1.3, "XY");
  gStyle->SetTitleOffset(1.2, "Y"); gStyle->SetTitleOffset(1.4, "X");
  gStyle->SetLabelSize(0.04) ;
  TGaxis::SetMaxDigits(3) ;

  // with BDT file
  TString histName[] = {"myMuMuKPiMassAltZoom","B0_multiplicity_AC"};
  histName[0].Append("_BDT"); histName[0].Append("_1B0");
  // with PROOF file
  //TString histName[] = {"myMuMuKPiMassCutsSelAlt","B0_multiplicity_AHC"};
  //TString histName[] = {"B0Mass_1B0_hardCuts","B0_multiplicity_AHC"}; 


  TH1F* histB0 = 0 ;

  Double_t meanval[3], sigma1val[3] ;
  meanval[0] = 5.28 ;  meanval[1] = 5.24 ;  meanval[2] = 5.32 ;
  //sigma1val[0] = 0.005 ; sigma1val[1] = 0.001 ; sigma1val[2] = 0.015 ;
  
  RooRealVar mean("m", "mean", meanval[0], meanval[1], meanval[2]) ;
  RooRealVar meanCB("m_{CB}", "mean CB", meanval[0], meanval[1], meanval[2]) ;
  Float_t sigmaMax = 0.05*10;
  RooRealVar sigma1("#sigma_{1}", "sigma1", sigmaMax/2, sigmaMax/200, sigmaMax);
  RooRealVar sigma2("#sigma_{2}", "sigma2", sigmaMax/2, sigmaMax/200, sigmaMax);

  TCanvas *Bpeak_C = new TCanvas("Bpeak_C","Bpeak_C", 800, 800) ;  
  TCanvas *Bmult_C = new TCanvas("Bmult_C","Bmult_C", 800, 600) ;  

  TString psi_nS = "psi2S"; psi_nS = "JPsi";
  
  Float_t B0_signal, innerSB, outerSB;
  B0_signal = 2.;
  //innerSB = 5. ; outerSB = 9.;
  //innerSB = 4. ; outerSB = 6.;
  innerSB = 6. ; outerSB = 9.;
  TString B0limits = TString::Format("%.1fSig_%.1fto%.1fSB",B0_signal,innerSB,outerSB);
  B0limits.ReplaceAll(".","p") ;

  vector <Float_t> bdt_cut;
  if (psi_nS.EqualTo("JPsi")) {
    //bdt_cut.push_back(0.); bdt_cut.push_back(0.05); bdt_cut.push_back(0.1);
    /*bdt_cut.push_back(-0.03);*/ bdt_cut.push_back(0.0); bdt_cut.push_back(0.05);
    //bdt_cut.push_back(-0.05); bdt_cut.push_back(0.0); bdt_cut.push_back(0.05); bdt_cut.push_back(0.1); bdt_cut.push_back(0.15); bdt_cut.push_back(0.2);
  } else if (psi_nS.EqualTo("psi2S")) {
    bdt_cut.push_back(0.); bdt_cut.push_back(0.05); bdt_cut.push_back(0.1);
  }

  cout <<"\nStarting loop over bdt_cut with bdt_cut.size() = " <<bdt_cut.size() <<endl;
  for (UInt_t iCut=0; iCut<bdt_cut.size(); iCut++) {

    TString outputName = TString::Format("BDTCutAt%.2f",bdt_cut[iCut]);
    outputName.ReplaceAll(".","p");
    TString fileName = "TMVApp_data_with"+outputName+"_"+psi_nS+"_"+B0limits+".root" ;
    if (!histName[0].Contains("BDT")) {
      fileName = "../Data_JPsi_2p0Sig_6p0to9p0SB.root";
      iCut = bdt_cut.size();
    }

    //TFile *target  = new TFile(fileName, "RECREATE");

    TString plotsDir = "plots/"+psi_nS+"/"+B0limits+"/vars_"+outputName;
    gSystem->mkdir(plotsDir, true);

    TFile *f = TFile::Open("../"+fileName);
    if ( f ) 
      cout <<"Opened file \"../"+fileName+"\"" <<endl;
    else {
      cout <<"\nWARNING! Unable to open file \"../"+fileName+"\"" <<endl;
      continue;
    }

    if ( !(histB0 = (TH1F*) f->Get(histName[0])) ) {
      cout <<"WARNING! Histogram \""+histName[0]+"\" not found in file \""+f->GetName()+"\"" <<endl;
      return;
    }

    Double_t xMin = histB0->GetXaxis()->GetXmin();
    Double_t xMax = histB0->GetXaxis()->GetXmax();
    Int_t nBins = histB0->GetNbinsX();
    Double_t bin_width = (xMax-xMin)/nBins ;
  
    RooRealVar xVar("xVar", histB0->GetXaxis()->GetTitle(), xMin, xMax) ;  xVar.setBins(nBins) ;  
    RooDataHist *MuMuPiKHist = new RooDataHist(histB0->GetName(), histB0->GetTitle(), RooArgSet(xVar), Import(*histB0, kFALSE)) ;
    RooConstVar nEntries("Entries", "Total number of entries", histB0->Integral()) ;
  
    Double_t fitMax = xMax, fitMin = xMin ;
  
    RooAbsPdf* sig1PDF=0, *sig2PDF=0;
    RooRealVar ratio("sig1frac", "fraction of component 1 in signal", 0.1, 0, 1);
    RooRealVar nSig1("N", "Number of 1^{st} signal", 1e+2, 1e+1, 3e+2);
    RooRealVar nSig2("N_{2}", "Number of 2^{nd} signal", 1e+5, 2e+3, 1e+7);

    Float_t alhpaIn = 1.5, alphaMin = 0, alphaMax = 20;
    RooRealVar alpha1("#alpha_{1}","alpha1", -alhpaIn, -alphaMax, alphaMin);
    RooRealVar alpha2("#alpha_{2}","alpha2",  alhpaIn,  alphaMin, alphaMax);
    RooRealVar n1("n_{1}","n1",1,0,100);
    RooRealVar n2("n_{2}","n2",1,0,100);

    RooAbsPdf* sigPDF = 0; TString func = "";
    Bool_t CB = kFALSE; //CB = kTRUE;
    Bool_t John = kFALSE; John = kTRUE;

    Bool_t twoPdfs = kFALSE, GaussCB = kFALSE, sameMean = kTRUE;
    if (histName[0].Contains("BDT") || histName[0].Contains("hardCuts") || histName[0].Contains("CutsSel")) {
      //twoPdfs = kTRUE;
      //GaussCB = kTRUE;
      sameMean = kFALSE;
    }

    cout <<"\nBuilding function according to CB = " <<CB <<", RooJohnson = " <<John <<" and twoPdfs = " <<twoPdfs <<endl;

    if (twoPdfs) {
      if (!CB) {
	sig1PDF = new RooGaussian("sig1PDF", "Signal component 1", xVar, mean, sigma1);
	sig2PDF = new RooGaussian("sig2PDF", "Signal component 2", xVar, mean, sigma2);
      }
      else {
	sig1PDF = new RooCBShape("sig1PDF", "Signal component 1", xVar, mean, sigma1, alpha1, n1);
	if (!GaussCB) {
	  sig2PDF = new RooCBShape("sig2PDF", "Signal component 2", xVar, mean, sigma2, alpha2, n2);
	  func = "_twoCB";
	  if (!sameMean)
	    sig1PDF = new RooCBShape("sig1PDF", "Signal component 1", xVar, meanCB, sigma1, alpha1, n1);
	} else {
	  sig2PDF = new RooGaussian("sig2PDF", "Signal component 2", xVar, mean, sigma2);
	  func = "_GaussCB";
	  if (!sameMean)
	    sig1PDF = new RooCBShape("sig1PDF", "Signal component 1", xVar, meanCB, sigma1, alpha1, n1);
	}
      }
      sigPDF = new RooAddPdf("sigPDF", "Signal component", RooArgList(*sig1PDF, *sig2PDF), ratio);
      //sigPDF = new RooAddPdf("sigPDF", "Signal component", RooArgList(*sig1PDF, *sig2PDF), RooArgList(nSig1, nSig2)) ;
    } 
    else { // one p.d.f.
      if (!CB) {
	if (!John)
	  sigPDF = new RooGaussian("sigPDF", "Signal component", xVar, mean, sigma1);
	else {
	  // The parameters δ and γ determine the shape of the distribution: for δ > 0, J(x;ξ,λ,γ,δ) is positive and normalized to one, and the sign of γ determines if the tail is located at low x (γ > 0) or at high x (γ < 0). Higher values of |δ| and |γ| result in a more symmetrical distribu- tion and a sharper peak. The parameters ξ and λ control the position and width of the distribution, respectively.
	  sigPDF = new RooJohnsonSU("sigPDF", "Signal component", xVar, mean, sigma1, alpha1, n1);
	  alpha1.SetNameTitle("#lambda","lambda");
	  n1.SetNameTitle("#delta","delta");
	  func = "_John";
	}
      } else { // Crystal ball
	sigPDF = new RooCBShape("sigPDF","Signal component", xVar, mean, sigma1, alpha1, n1); alpha1.SetNameTitle("#alpha","alpha");
	func = "_CB";
      }
      sigma1.SetNameTitle("#sigma","sigma");
    }

    if (twoPdfs && !sameMean)
      func.Append("_diffMeans");

    RooRealVar nSig("N_{Sig}", "Number of signal candidates", 2.5e+3, 1e+2, 1e+6);
    //RooFormulaVar nSig("N_{Sig}", "Number of signal candidates", "@0+@1", RooArgList(nSig1, nSig2));
    /*// not with Kai BF
      RooGaussian bumpPDF("bumpPDF", "Bump", xVar, mean3, sigma3);
      RooRealVar nBump("N_{3}", "Number of bump candidates", 1e+5, 1e+3, 1e+7);
    */
    RooRealVar c1("c_{1}", "c1", +0.02, -10, 10);
    //RooRealVar c1("c_{1}", "c1", -0.02, -10, 10);
    //RooChebychev bkgPDF("bkgPDF", "bkgPDF", xVar, RooArgSet(c1));  // with 5 sigma
    RooRealVar c2("c_{2}", "c2", -0.001, -10, 10);
    RooArgSet chebCoeff(c1, c2);
    //RooChebychev bkgPDF("bkgPDF", "bkgPDF", xVar, RooArgSet(c1,c2));
    RooRealVar c3("c_{3}", "c3", -0.001, -10, 10);
    //RooChebychev bkgPDF("bkgPDF", "bkgPDF", xVar, RooArgSet(c1,c2,c3));
    RooRealVar c4("c_{4}", "c4", -0.001, -10, 10);
    //RooChebychev bkgPDF("bkgPDF", "bkgPDF", xVar, RooArgSet(c1,c2,c3,c4));
    RooRealVar c5("c_{5}", "c5", -0.001, -10, 10);
    if (histName[0].Contains("BDT") || histName[0].Contains("hardCuts")) {
      chebCoeff.add(c3); chebCoeff.add(c4); //chebCoeff.add(c5);
    } 
    RooChebychev bkgPDF("bkgPDF", "bkgPDF", xVar, chebCoeff);

    /* // not with Kai BF
       RooRealVar gamma("#Gamma", "gamma", -0.01, -5, -0.0005);
       RooExponential bkgPDF("bkgPDF", "Background", xVar, gamma) ;
    */
    RooRealVar nBkg("nBkg", "Bkg component", 1e+5, 1e+2, 1e+8);     
  
    //RooAddPdf *totalPDF = new RooAddPdf("totalPDF", "totalPDF", RooArgList(sigPDF, bkgPDF), RooArgList(nSig, nBkg));  // with 5 sigma
    RooAddPdf *totalPDF = new RooAddPdf("totalPDF", "totalPDF", RooArgList(*sigPDF, bkgPDF), RooArgList(nSig, nBkg));  // with 5 sigma
    //RooAddPdf *totalPDF = new RooAddPdf("totalPDF", "totalPDF", RooArgList(*sig1PDF, *sig2PDF, bkgPDF), RooArgList(nSig1, nSig2, nBkg));     
    //RooAddPdf *totalPDF = new RooAddPdf("totalPDF", "totalPDF", RooArgList(sigPDF, bumpPDF, bkgPDF), RooArgList(nSig, nBump, nBkg));  // not with Kai BF 
  
    cout <<"\n\n---------------  Fitting histo \"" <<MuMuPiKHist->GetTitle() <<"\"  ---------------\n" <<endl;
  
    totalPDF->fitTo(*MuMuPiKHist, Range(fitMin, fitMax), Extended(kTRUE));
  
    Float_t sigmaMin = TMath::Min(sigma1.getVal(),sigma2.getVal());
    TString sigmaMin_name = sigma1.GetName();
    if (sigma1.getVal() > sigma2.getVal())
      sigmaMin_name = sigma2.GetName();
 
    if (GaussCB) { // setting Gaussian sigma
      sigmaMin = sigma2.getVal();
      sigmaMin_name = sigma2.GetName();
    }

    Float_t fitMean = mean.getVal();
    Float_t nSigma = 2; //nSigma = 3;
    if (John) nSigma = 1.;
    Bool_t fixedRange = kTRUE; fixedRange = kFALSE;
    if (!fixedRange) {
      xVar.setRange("signal", fitMean - nSigma*sigmaMin, fitMean + nSigma*sigmaMin) ;
    } else {
      xVar.setRange("signal", B0mass -nSigma*(0.012), B0mass +nSigma*0.012) ;
      xVar.setRange("signal", B0mass -0.02, B0mass +0.02) ;
    }
    RooAbsReal* tot_sigRange = totalPDF->createIntegral(xVar, NormSet(xVar), Range("signal")) ;
    RooAbsReal* sig_sigRange = sigPDF->createIntegral(xVar, NormSet(xVar), Range("signal")) ;

    RooPlot* xFrame = xVar.frame();
    //xFrame->SetAxisRange(fitMin, xMax) ;
    xFrame->SetTitle( MuMuPiKHist->GetTitle() ) ;
    xFrame->SetYTitle( TString::Format("Candidates / %.1f MeV/c^{2}", bin_width*1000) ) ;
  
    MuMuPiKHist->plotOn(xFrame, Name("data"));
    totalPDF->plotOn(xFrame, Name("totalPDF"));
    
    RooArgSet paramsToShow = RooArgSet(nSig,mean,sigma1);

    if (CB || John) {
      paramsToShow.add(alpha1); paramsToShow.add(n1);
    }
    if (twoPdfs) {
      totalPDF->plotOn(xFrame, Components(RooArgSet(*sig1PDF)), LineColor(kMagenta));
      totalPDF->plotOn(xFrame, Components(RooArgSet(*sig2PDF)), LineColor(kOrange));
      paramsToShow.add(sigma2);
      if (CB) {
	if (!GaussCB) {
	  paramsToShow.add(alpha2); //paramsToShow.add(n2);
	}
      }
      if (!sameMean)
	paramsToShow.add(meanCB);
    } else {
      //totalPDF->plotOn(xFrame, Components(RooArgSet(sigPDF)), LineColor(kMagenta));
      totalPDF->plotOn(xFrame, Components(RooArgSet(*sigPDF)), LineColor(kMagenta));
    }
    
    //totalPDF->plotOn(xFrame, Components(RooArgSet(sigPDF)), LineColor(kRed));  // not with Kai BF 
    //totalPDF->plotOn(xFrame, Components(RooArgSet(sig2PDF)), LineColor(kMagenta));
    //totalPDF->plotOn(xFrame, Components(RooArgSet(bumpPDF)), LineColor(kGreen));  // not with Kai BF 
    totalPDF->plotOn(xFrame, Components(RooArgSet(bkgPDF)), LineColor(kBlue), LineStyle(2));  // with Kai BF

    // Purity
    cout <<"\nGaussian mean = " <<fitMean <<endl;
    cout <<"Gaussian " <<sigma1.GetTitle() <<" = " <<sigma1.getVal() <<endl;
    if (!fixedRange)
      cout <<"\nDefining the signal region as mean +/-" <<nSigma <<" " <<sigma1.GetTitle() <<":" <<endl;
    else
      cout <<"\nDefining the signal region as m(B0) +/- 20 MeV:" <<endl;
    Double_t tot_sigRange_val = tot_sigRange->getVal() ; // compute integral
    cout <<"\nIntegral of total PDF in signal region = " << nEntries.getVal()*tot_sigRange_val << endl;
    Float_t purEvents = nSig.getVal()*(sig_sigRange->getVal());
    cout <<"\nIntegral of signal PDF in signal region = " << purEvents << endl;
    cout <<"\nBackground events in signal region = " << nEntries.getVal()*tot_sigRange_val << " - " << purEvents << " = " << nEntries.getVal()*tot_sigRange_val - purEvents << endl;
    Double_t purity = 100 * purEvents / (nEntries.getVal()*tot_sigRange_val);
    cout <<"Purity is = " << purity <<"%" << endl;

    TString purity_dynamic = TString::Format("Purity in m +/- %.0f%s", nSigma, sigmaMin_name.Data());
    TString purity_fixed = "Purity in m(B^{0}) +/- 20 MeV";
    TString purity_label = purity_dynamic;
    if (fixedRange) purity_label = purity_fixed;

    Float_t yStatMax = 0.9;
    if (!John) yStatMax = 0.95;
    //totalPDF->paramOn(xFrame, Parameters(paramsToShow), Layout(0.55,0.9,yStatMax)) ;
    totalPDF->paramOn(xFrame, Parameters(paramsToShow), ShowConstants(kTRUE), Label(TString::Format("%s: %.1f%%", purity_label.Data(), purity)), Layout(0.52,0.98,yStatMax)) ;
    /*
      if (fitMin < 4.8) {
      //totalPDF->paramOn(xFrame, Parameters(RooArgSet(nSig1,nSig2,mean1,mean2,sigma1,sigma2)), Layout(0.41,0.74,0.92)) ;
      totalPDF->paramOn(xFrame, Parameters(paramsToShow), Layout(0.41,0.74,0.85)) ;
      } else if (fixedMassWind != "_5sigma") {
      //totalPDF->paramOn(xFrame, Parameters(paramsToShow), Layout(0.17,0.52,0.8)) ;
      //} else {
      totalPDF->paramOn(xFrame, Parameters(paramsToShow), Layout(0.6,0.98,yStatMax)) ;
      }
    */

    Bpeak_C->cd();
    Float_t padsSeparation = 0.2;
    TPad *pad1 = new TPad("pad1", "Upper pad",0.0,padsSeparation,1.0,1.0); pad1->SetBottomMargin(0.05);
    TPad *pad2 = new TPad("pad2", "Lower pad",0.0,0.0,1.0,padsSeparation); pad2->SetTopMargin(0.05); pad2->SetBottomMargin(0.2);
    pad1->Draw();
    pad2->Draw();

    pad1->cd();
    xFrame->Draw();

    if (!histName[0].Contains("1B0")) { // adding all - 1B0 difference
      TString hist1B0_name = histName[0]+"_1B0";
      TH1F* hist1B0 = 0;

      if (!fileName.Contains("TMVA"))
	hist1B0_name = "B0Mass_1B0_hardCuts";

      TH1F* histDiff = 0;
      if ( (hist1B0 = (TH1F*) f->Get(hist1B0_name)) )
	histDiff = (TH1F*) histB0->Clone(histName[0]+"_moreB0");
      else
	cout <<"WARNING! Hisotgram \"" <<hist1B0_name <<"\" not found in TFile " <<fileName <<endl;
      if (histDiff) {
	cout <<"\nAdding multi-B0 candidates histogram" <<endl;
	histDiff->Add(hist1B0, -1);
	histDiff->SetFillColor(kRed); histDiff->Draw("same");
      }
    } else { //if (histName[0].Contains("1B0"))
      cout <<"\nAdding purity and sidebnds boundaries" <<endl;
      //nSigma = 1.5;
      if (!CB && !John && outputName.Contains("At0p05")) {
	sigmaMin = 0.015;
        //
        Float_t area1 = ratio.getVal()*nSig.getVal();
        Float_t area2 = nSig.getVal() - area1;
        Float_t sigma_mean = (area1*sigma1.getVal() + area2*sigma2.getVal()) / nSig.getVal();
        cout <<"\nWeighted mean is = (" <<area1 <<"*" <<sigma1.getVal() <<" + " <<area2 <<"*" <<sigma2.getVal() <<") / " <<nSig.getVal() <<" = " <<sigma_mean <<endl;
      }
      TString rangeMean = mean.GetName();
      Float_t leftSig = fitMean - nSigma*sigmaMin;
      Float_t rightSig = fitMean + nSigma*sigmaMin;
      if (John) {
	leftSig = B0mass - nSigma*sigmaMin;
	rightSig = B0mass + nSigma*sigmaMin;
	rangeMean = "m_{B^{0}}";
      }
      xVar.setRange("signal", leftSig, fitMean +nSigma*sigmaMin) ;
      tot_sigRange = totalPDF->createIntegral(xVar, NormSet(xVar), Range("signal")) ;
      sig_sigRange = sigPDF->createIntegral(xVar, NormSet(xVar), Range("signal")) ;

      tot_sigRange_val = tot_sigRange->getVal() ; // compute integral
      cout <<"\nIntegral of total PDF in signal region = " << nEntries.getVal()*tot_sigRange_val << endl;
      purEvents = nSig.getVal()*(sig_sigRange->getVal());
      cout <<"\nIntegral of signal PDF in signal region = " << purEvents << endl;
      cout <<"\nBackground events in signal region = " << nEntries.getVal()*tot_sigRange_val << " - " << purEvents << " = " << nEntries.getVal()*tot_sigRange_val - purEvents << endl;
      purity = 100 * purEvents / (nEntries.getVal()*tot_sigRange_val);
      cout <<"Purity is = " << purity <<"%" << endl;

      purity_label = TString::Format("Purity in %s +/- %.1f%s", rangeMean.Data(), nSigma, sigmaMin_name.Data()); 
      Float_t xStatMin = 0.58, xStatMax = 0.98;
      if (!CB && !John && outputName.Contains("At0p05")) {
	purity_label.ReplaceAll(sigmaMin_name,"*(15 MeV)");
	xStatMin -= 0.05; }
      xFrame->remove();
      totalPDF->paramOn(xFrame, Parameters(paramsToShow), ShowConstants(kTRUE), Label(TString::Format("%s: %.1f%%", purity_label.Data(), purity)), Layout(xStatMin,xStatMax,yStatMax)) ;

      xFrame->Draw();

      Float_t factor = 1.7;

      Float_t sigHeight = histB0->GetBinContent(histB0->FindBin(leftSig));
      if (sigHeight < histB0->GetBinContent(histB0->FindBin(rightSig)))
	sigHeight = histB0->GetBinContent(histB0->FindBin(rightSig));
      gPad->Update(); Float_t frameMax = gPad->GetUymax();
      Float_t sigLMax = TMath::Min(sigHeight*factor, frameMax);
      TLine* leftSigL = new TLine(leftSig, 0, leftSig, sigLMax); leftSigL->SetLineColor(kGreen);
      TLine* rightSigL = new TLine(rightSig, 0, rightSig, sigLMax); rightSigL->SetLineColor(kGreen);

      Float_t innerSb = 3.5*sigmaMin, outerSb = 4.5*sigmaMin;
      Float_t leftSbIn = fitMean -innerSb; Float_t rightSbIn = fitMean +innerSb;
      Float_t leftSbOut = fitMean -outerSb; Float_t rightSbOut = fitMean +outerSb;
      Float_t sbHeight = histB0->GetBinContent(histB0->FindBin(leftSbIn));
      if (sbHeight < histB0->GetBinContent(histB0->FindBin(rightSbIn)))
	sbHeight = histB0->GetBinContent(histB0->FindBin(rightSbIn));
      if (sbHeight < histB0->GetBinContent(histB0->FindBin(leftSbOut)))
	sbHeight = histB0->GetBinContent(histB0->FindBin(leftSbOut));
      if (sbHeight < histB0->GetBinContent(histB0->FindBin(rightSbOut)))
	sbHeight = histB0->GetBinContent(histB0->FindBin(rightSbOut));
      TLine* leftSbInL = new TLine(leftSbIn, 0, leftSbIn, sbHeight*factor); leftSbInL->SetLineColor(kRed);
      TLine* rightSbInL = new TLine(rightSbIn, 0, rightSbIn, sbHeight*factor); rightSbInL->SetLineColor(kRed);
      TLine* leftSbOutL = new TLine(leftSbOut, 0, leftSbOut, sbHeight*factor); leftSbOutL->SetLineColor(kRed);
      TLine* rightSbOutL = new TLine(rightSbOut, 0, rightSbOut, sbHeight*factor); rightSbOutL->SetLineColor(kRed);
      if (John) { // asymmetric sidebands
	leftSbInL->SetX1(leftSbInL->GetX1() + sigmaMin); leftSbInL->SetX2(leftSbInL->GetX2() + sigmaMin);
	rightSbInL->SetX1(rightSbInL->GetX1() + 0.5*sigmaMin); rightSbInL->SetX2(rightSbInL->GetX2() + 0.5*sigmaMin);
	rightSbOutL->SetX1(rightSbOutL->GetX1() + 1.5*sigmaMin); rightSbOutL->SetX2(rightSbOutL->GetX2() + 1.5*sigmaMin);
      }

      leftSbOutL->SetX1( TMath::Max(leftSbOutL->GetX1(), xMin) ); leftSbOutL->SetX2( TMath::Max(leftSbOutL->GetX2(), xMin) );
      rightSbOutL->SetX1( TMath::Min(rightSbOutL->GetX1(), xMax) ); rightSbOutL->SetX2( TMath::Min(rightSbOutL->GetX2(), xMax) );

      leftSigL->SetLineWidth(3); rightSigL->SetLineWidth(3);
      leftSbInL->SetLineWidth(3); rightSbInL->SetLineWidth(3);
      leftSbOutL->SetLineWidth(3); rightSbOutL->SetLineWidth(3);
      leftSigL->Draw(); rightSigL->Draw();
      leftSbInL->Draw(); rightSbInL->Draw();
      leftSbOutL->Draw(); rightSbOutL->Draw();

    }

    pad2->cd();
    RooHist* hPull = xFrame->pullHist("data","totalPDF") ; hPull->GetYaxis()->SetTitle("Pull"); hPull->GetYaxis()->SetTitleSize();
    RooPlot* framePull = xVar.frame() ; framePull->SetTitle("");
    framePull->addPlotable(hPull,"P") ;
    Float_t maxPull = 5.; framePull->SetMinimum(-maxPull); framePull->SetMaximum(maxPull);
    framePull->SetYTitle("Pull");
    framePull->GetYaxis()->SetTitleSize( framePull->GetYaxis()->GetTitleSize() / (1.3*padsSeparation) ); framePull->GetYaxis()->SetTitleOffset( 1.5 * padsSeparation );
    framePull->GetYaxis()->SetNdivisions(5*100 + maxPull, kFALSE);
    framePull->GetYaxis()->SetLabelSize( framePull->GetYaxis()->GetLabelSize() / (1.3*padsSeparation) ); //framePull->GetYaxis()->SetTitleOffset( 1.5 * padsSeparation );
    framePull->GetXaxis()->SetTitleSize( framePull->GetXaxis()->GetTitleSize() / (1.5*padsSeparation) ); framePull->GetXaxis()->SetTitleOffset( 3 * padsSeparation );
    //framePull->GetXaxis()->SetTitleSize( 20 );
    framePull->Draw();

    Float_t pullLine = 3;
    gPad->Update();
    TLine* topL = new TLine(gPad->GetUxmin(),pullLine,gPad->GetUxmax(),pullLine);
    TLine* zeroL = new TLine(gPad->GetUxmin(),0,gPad->GetUxmax(),0);
    TLine* bottomL = new TLine(gPad->GetUxmin(),-pullLine,gPad->GetUxmax(),-pullLine);
    topL->SetLineColor(kRed); zeroL->SetLineColor(kBlue); bottomL->SetLineColor(kRed);
    topL->SetLineStyle(kDashed); zeroL->SetLineStyle(kDashed); bottomL->SetLineStyle(kDashed);
    topL->Draw(); zeroL->Draw(); bottomL->Draw();


    TString plotName = histName[0]; plotName.ReplaceAll("BDT",outputName.Data());
    Bpeak_C->SaveAs( TString::Format("%s/%s%s.png", plotsDir.Data(), plotName.Data(), func.Data() ) );

    ////////// removing twins //////////
    /*
      TH1F* hB0Mass_twins = (TH1F*) f->Get("B0Mass_twins") ;
      TH1F* diff_hist = (TH1F*) histB0->Clone("diff_hist"); diff_hist->Sumw2();
      diff_hist->Add(hB0Mass_twins, -1);

      RooDataHist *diff_RooHist = new RooDataHist("diff_RooHist", "B^{0} mass without twins", RooArgSet(xVar), Import(*diff_hist, kFALSE)) ;

      totalPDF->fitTo(*diff_RooHist, Range(fitMin, fitMax), Extended(kTRUE));

      RooPlot* xFrameDiff = xFrame->emptyClone("xFrameDiff") ;
      xFrameDiff->SetTitle( diff_RooHist->GetTitle() ) ;
  
      diff_RooHist->plotOn(xFrameDiff);
      totalPDF->plotOn(xFrameDiff);
      totalPDF->plotOn(xFrameDiff, Components(RooArgSet(sigPDF)), LineColor(kRed));
      //totalPDF->plotOn(xFrameDiff, Components(RooArgSet(sig2PDF)), LineColor(kMagenta));
      //totalPDF->plotOn(xFrameDiff, Components(RooArgSet(sigPDF)), LineColor(kRed));  // not with Kai BF 
      //totalPDF->plotOn(xFrameDiff, Components(RooArgSet(sig2PDF)), LineColor(kMagenta));
      //totalPDF->plotOn(xFrameDiff, Components(RooArgSet(bumpPDF)), LineColor(kGreen));  // not with Kai BF 
      totalPDF->plotOn(xFrameDiff, Components(RooArgSet(bkgPDF)), LineColor(kBlue), LineStyle(2));  // with Kai BF

      totalPDF->paramOn(xFrameDiff, Parameters(paramsToShow), ShowConstants(kTRUE), Layout(0.55,0.9,0.9)) ;

      xFrameDiff->Draw();

      //Bpeak_C->SaveAs("plots/b_peak/B0mass_withoutTwins.png");
      */

    // multiplicity plot
    TH1I* histB0_mult = (TH1I*) f->Get(histName[1]) ;
    histB0_mult->SetBinContent(1,0);

    Bmult_C->cd();
    histB0_mult->Draw(); histB0_mult->SetLineWidth(2);
    gPad->SetLogy();
    histB0_mult->Draw("text30 same");

    Float_t multiCand = (histB0_mult->Integral() - histB0_mult->GetBinContent(histB0_mult->FindBin(1))) / (Float_t)(histB0_mult->Integral());
    cout <<"\nThe fraction of events with more than one B0 candidates is " <<multiCand*100 <<"%\n" <<endl;
    plotName = histName[1]; plotName.ReplaceAll("BDT",outputName.Data());
    Bmult_C->SaveAs( TString::Format("%s/%s.png", plotsDir.Data(), plotName.Data()) );

  } //   for (UInt_t iCut=0; iCut<bdt_cut.size(); iCut++) {
}
//rsync -vut --existing b_peakFit.C cristella@cmssusy.ba.infn.it:/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/sanjay/selector/b_peak/b_peakFit.C
