#include <TStyle.h>
#include <TGaxis.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TPaveText.h>
#include <TLegend.h>

//gInterpreter->AddIncludePath("/cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/roofit/5.34.09-cms5/include") ;
//gROOT->ProcessLine(".include /cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/roofit/5.34.09-cms5/include") ;
// one of the 2 above lines need to be executed interactively before including any Roo*.h
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooConstVar.h>
#include <RooGaussian.h>
#include <RooChebychev.h>
#include <RooAddPdf.h>
#include <RooPlot.h>

using namespace RooFit; // needed even without .C+

// lines above needed with .C+
/*
gSystem->Load("libRooFit");
using namespace RooFit; // needed even without .C+
*/

// usage: root -l roc.C+
void roc() {

  gStyle->SetOptStat(0);
  //gStyle->SetTitleOffset(1.3, "XY");
  gStyle->SetTitleOffset(1.2, "Y"); gStyle->SetTitleOffset(1.4, "X");
  gStyle->SetLabelSize(0.04) ;
  TGaxis::SetMaxDigits(3) ;

  TString fileName_sig = "TightCuts_histos_officialMC";
  //TString fileName_bkg = "TightCuts_histos_Data_fullStatistics";
  TString fileName_bkg = "TightCuts_histos_Data";
  TFile *sig_file = TFile::Open(fileName_sig+".root");
  TFile *bkg_file = TFile::Open(fileName_bkg+".root");

  if (!bkg_file) {
    std::cout <<"\nError: could not open file: " <<fileName_bkg.Data() <<std::endl ;
    return;
  }
  if (!sig_file) {
    std::cout <<"\nError: could not open file: " <<fileName_sig.Data() <<std::endl ;
    return;
  }

  TString dir = "";

  // compute signal/background yields
  TH1F* B0_sig = 0;
  Double_t nB0Background = 0.; 
  if ( (B0_sig = (TH1F*) sig_file->Get("myPsiPKPiMassAltZoom")) ) {

    Double_t xMin = B0_sig->GetXaxis()->GetXmin();
    Double_t xMax = B0_sig->GetXaxis()->GetXmax();
    Int_t nBins = B0_sig->GetNbinsX();
    Double_t bin_width = (xMax-xMin)/nBins ;
  
    RooRealVar xVar("xVar", B0_sig->GetXaxis()->GetTitle(), xMin, xMax) ;  xVar.setBins(nBins) ;  
    RooDataHist *RooPsiPKPiMassSelAltZoom = new RooDataHist(B0_sig->GetName(), B0_sig->GetTitle(), RooArgSet(xVar), Import(*B0_sig, kFALSE)) ;
    RooConstVar nEntries("Entries", "Total number of entries", B0_sig->Integral()) ;
  
    Double_t fitMax = xMax, fitMin = xMin ;

    RooRealVar mean("m", "mean", 5.28, 5.25, 5.31) ;
    RooRealVar sigma1("#sigma_{1}", "sigma1", 0.02, 0.01, 0.05);
    RooRealVar sigma2("#sigma_{2}", "sigma2", 0.02, 0.05, 0.2);

    RooAbsPdf* sigPDF = 0;
    Int_t nGaussian = 1; nGaussian = 2;
    RooGaussian* sig1PDF = 0; RooGaussian* sig2PDF = 0;
    if (nGaussian == 1) {
      sigPDF = new RooGaussian("sigPDF", "Signal component", xVar, mean, sigma1); sigma1.SetNameTitle("#sigma","sigma");
    }
    else if (nGaussian == 2) {
      sig1PDF = new RooGaussian("sig1PDF", "Signal component 1", xVar, mean, sigma1);
      sig2PDF = new RooGaussian("sig2PDF", "Signal component 2", xVar, mean, sigma2);
      RooRealVar ratio("sig1frac", "fraction of component 1 in signal", 0.1, 0, 0.2);
      sigPDF = new RooAddPdf("sigPDF", "Signal component", RooArgList(*sig1PDF, *sig2PDF), ratio);
    }
    RooRealVar nSig("N_{Sig}", "Number of signal candidates", 2.5e+3, 1e+2, 1e+5);

    RooRealVar c1("c_{1}", "c1", +0.02, -10, 10);
    RooRealVar c2("c_{2}", "c2", -0.001, -10, 10);
    RooChebychev bkgPDF("bkgPDF", "bkgPDF", xVar, RooArgSet(c1,c2));
    RooRealVar nBkg("nBkg", "Bkg component", 1e+6, 1e+2, 1e+8);     
  
    RooAddPdf *totalPDF = new RooAddPdf("totalPDF", "totalPDF", RooArgList(*sigPDF, bkgPDF), RooArgList(nSig, nBkg));  // with 5 sigma
    std::cout <<"\n\n---------------  Fitting histo \"" <<RooPsiPKPiMassSelAltZoom->GetTitle() <<"\"  ---------------\n" <<endl;
	
    totalPDF->fitTo(*RooPsiPKPiMassSelAltZoom, Range(fitMin, fitMax), Extended(kTRUE));
	
    Float_t nSigma = 2; //nSigma = 3;
    xVar.setRange("signal", mean.getVal() -nSigma*(0.012), mean.getVal() +nSigma*0.012) ;
    RooAbsReal* tot_sigRange = totalPDF->createIntegral(xVar, NormSet(xVar), Range("signal")) ;
    RooAbsReal* sig_sigRange = sigPDF->createIntegral(xVar, NormSet(xVar), Range("signal")) ;
	
    RooPlot* xframe = xVar.frame();
    //xframe->SetAxisRange(fitMin, xMax) ;
    xframe->SetTitle( RooPsiPKPiMassSelAltZoom->GetTitle() ) ;
    xframe->SetYTitle( TString::Format("Candidates / %.1f MeV/c^{2}", bin_width*1000) ) ;

    RooPsiPKPiMassSelAltZoom->plotOn(xframe);
    totalPDF->plotOn(xframe); 	
    if (nGaussian == 1)
      totalPDF->plotOn(xframe, Components(RooArgSet(*sigPDF)), LineColor(kRed)); 
    else if (nGaussian == 2) {
      if (!sig1PDF)
	totalPDF->plotOn(xframe, Components(RooArgSet(*sig1PDF)), LineColor(kOrange));
      if (!sig2PDF)
	totalPDF->plotOn(xframe, Components(RooArgSet(*sig2PDF)), LineColor(kMagenta));
    }
    totalPDF->plotOn(xframe, Components(RooArgSet(bkgPDF)), LineColor(kBlue), LineStyle(2));

    if (nGaussian == 1)
      totalPDF->paramOn(xframe, Parameters(RooArgSet(nSig,mean,sigma1)), ShowConstants(kTRUE), Layout(0.55,0.9,0.9)) ;
    else if (nGaussian == 2)
      totalPDF->paramOn(xframe, Parameters(RooArgSet(nSig,mean,sigma1,sigma2)), Layout(0.55,0.9,0.9)) ;

    xframe->Draw();
    gPad->SaveAs( TString::Format("plots/%s/ROC/B0_mass_1B0_fit.png", dir.Data()) );

    std::cout <<"\nGaussian mean = " <<mean.getVal() <<endl;
    std::cout <<"Gaussian sigma = " <<sigma1.getVal() <<endl;
    std::cout <<"\nDefining the signal region as mean +/-" <<nSigma <<" sigma:" <<endl;
    Double_t tot_sigRange_val = tot_sigRange->getVal() ; // compute integral
    std::cout <<"\nIntegral of total PDF in signal region = " << nEntries.getVal()*tot_sigRange_val << endl;
    std::cout <<"\nIntegral of signal PDF in signal region = " << nSig.getVal()*(sig_sigRange->getVal()) << endl;
    nB0Background = nEntries.getVal()*tot_sigRange_val - nSig.getVal()*(sig_sigRange->getVal());
    std::cout <<"\nB0Background events in signal region = " << nEntries.getVal()*tot_sigRange_val << " - " << nSig.getVal()*(sig_sigRange->getVal()) << " = " << nB0Background << endl;
    std::cout <<"Purity is = " << 100 * nSig.getVal()*(sig_sigRange->getVal()) / (nEntries.getVal()*tot_sigRange_val) <<"%" << endl;

  } else Warning("Terminate", "histogram \"myPsiPKPiMassAltZoom\" not found");


  // ROC
  vector< pair<TString, TString> > nameROC_TH1F ;
  nameROC_TH1F.push_back( make_pair("kaonPt","K^{+} p_{T}") );
  nameROC_TH1F.push_back( make_pair("pionPt","#pi^{-} p_{T}") );
  nameROC_TH1F.push_back( make_pair("B0Pt","B^{0} p_{T}") );
  nameROC_TH1F.push_back( make_pair("B0VtxCL","B^{0} vtx CL") );
  nameROC_TH1F.push_back( make_pair("B0_PVCTau","c*#tau_{PV} / #sigma(c*#tau_{PV})") );
  nameROC_TH1F.push_back( make_pair("B0_BSCTau","c*#tau_{BS} / #sigma(c*#tau_{BS})") );
  nameROC_TH1F.push_back( make_pair("B0_pointingAnglePV","B^{0} cos(#alpha) wrt PV") );
  
  Int_t yOffsetFactor = 1;
  TString label[2], sample[2];
  label[0] = "signal"; sample[0] = "MC";
  label[1] = "bkg"; sample[1] = "Data";
  //
  for (Int_t iVar = 0; iVar < nameROC_TH1F.size(); ++iVar) {
    TString nameSig = nameROC_TH1F[iVar].first+"_fromB0peak";
    TString nameSb = nameROC_TH1F[iVar].first+"_fromB0sb";
    TH1F* histoPeak = 0; TH1F* histoSb = 0;
    
    if ( (histoPeak = (TH1F*) sig_file->Get(nameSig)) ) {
      if ( (histoSb = (TH1F*) bkg_file->Get(nameSb)) ) {
	
	TH1F* hB0Signal = (TH1F*) histoPeak->Clone(nameROC_TH1F[iVar].first+"_sbSubtr") ; hB0Signal->Sumw2() ;
	histoSb->Scale(nB0Background/histoSb->Integral());
	hB0Signal->Add(histoSb, -1) ;
	hB0Signal->SetTitle( nameROC_TH1F[iVar].second+" sidebands subtracted" ) ;
	
	TH1F *hVar[2];
	hVar[0] = hB0Signal; 
	hVar[1] = histoSb;
	hVar[1]->Scale(hVar[0]->Integral()/hVar[1]->Integral());
	TH1F *hRatio[2];
	//
	Double_t TotIntegral[2];
	Double_t PartIntegral[2];
	Double_t Eff[2];
	Double_t IntegralCounts[2][500];
	//
	Int_t nBins = 0;
	vector< pair<Int_t,TString> > cutLabel;
	for (int i=0; i<2; i++) {
	  nBins = hVar[i]->GetNbinsX();
	  TotIntegral[i] = hVar[i]->Integral(1,nBins);
	  std::cout << "\nVariable: " <<nameROC_TH1F[iVar].first <<" " <<sample[i] <<" " <<label[i] <<endl;
	  std::cout << "# of bins = " << nBins << endl;
	  hRatio[i] = new TH1F(TString::Format("%s_ratio_%s_%s",hVar[i]->GetName(),sample[i].Data(),label[i].Data()),"Partial integral over total for "+sample[i]+" "+label[i],nBins,0.,1.);
	  for(int j=1; j<nBins+1; j++)
	    {
	      PartIntegral[i] = hVar[i]->Integral(j,nBins);
	      Eff[i] = PartIntegral[i]/TotIntegral[i];
	      IntegralCounts[i][j-1] = Eff[i];
	      if ( j==nBins ) std::cout << "i = " << i << " , j = " << j << " : eff1 = " << IntegralCounts[i][j-1] << endl;
	      hRatio[i]->Fill(Eff[i]); // non estremamente significativo; per i=0/segnale picca ad 1
	      //
	      if (i==0)
		if (!( (j-1+10)%(nBins/5) )) {
		  if (!(nameROC_TH1F[iVar].first.Contains("pointingAngle")))
		    cutLabel.push_back( make_pair(j,TString::Format("%s > %.1f",hVar[i]->GetXaxis()->GetTitle(),hVar[i]->GetBinLowEdge(j))) );
		  else
		    cutLabel.push_back( make_pair(j,TString::Format("%s > %.4f",hVar[i]->GetXaxis()->GetTitle(),hVar[i]->GetBinLowEdge(j))) );
		}
	    }
	}
	
	//cout <<"\nGoing to Draw" <<endl;
	TGraph *var_ROC = new TGraph(nBins,IntegralCounts[1],IntegralCounts[0]);
	var_ROC->SetTitle(nameROC_TH1F[iVar].second+" ROC;B^{0} background efficiency;B^{0} signal efficiency;");
	TCanvas *var_C = new TCanvas("var_C",nameROC_TH1F[iVar].second+" distributions", 2*600, 2*400); var_C->Divide(2,2);
	var_C->cd(1);
	histoPeak->SetTitle( sample[0]+" "+histoPeak->GetTitle() );
	histoSb->SetTitle( sample[1]+" "+histoSb->GetTitle() );
	if (histoPeak->GetMaximum() > histoSb->GetMaximum()) {
	  histoPeak->Draw(); histoSb->Draw("same"); }
	else {
	  histoSb->Draw(); histoPeak->Draw("same"); }
	histoSb->SetLineColor(kRed);
	TLegend* leg = gPad->BuildLegend(0.35, 0.67, 0.87, 0.88) ; // with CMSSW_5_3_22 ROOT version gives error: 'class TVirtualPad' has no member named 'BuildLegend'
	leg->SetTextSize(0.04);
	histoPeak->SetTitle(nameROC_TH1F[iVar].second); histoSb->SetTitle(nameROC_TH1F[iVar].second);
	var_C->cd(2);
	hB0Signal->Draw();
	var_C->cd(3);
	if (hRatio[0]->GetMaximum() > hRatio[1]->GetMaximum()) {
	  hRatio[0]->Draw(); hRatio[1]->Draw("same"); gPad->BuildLegend(0.4, 0.67, 0.85, 0.88); hRatio[0]->SetTitle("Partial integral over total"); }
	else {
	  hRatio[1]->Draw(); hRatio[0]->Draw("same"); gPad->BuildLegend(0.4, 0.67, 0.85, 0.88); hRatio[1]->SetTitle("Partial integral over total"); }
	hRatio[1]->SetLineColor(kRed);
	var_C->cd(4);
	var_ROC->Draw("AC*");
	//cout <<"\nDrawing cut labels" <<endl;
	for (Int_t j=0; j<=cutLabel.size()/2; ++j) {
	  Int_t iCutLabel = j;
	  if (nameROC_TH1F[iVar].first.Contains("pointingAngle") || nameROC_TH1F[iVar].first.Contains("B0Pt")) iCutLabel = j + cutLabel.size()/2;
	  if (nameROC_TH1F[iVar].first.Contains("pointingAngle")) yOffsetFactor = j+1;
	  TPaveText *pt = new TPaveText(IntegralCounts[1][cutLabel[iCutLabel].first] +0.03,IntegralCounts[0][cutLabel[iCutLabel].first]-0.07 -0.04*yOffsetFactor,IntegralCounts[1][cutLabel[iCutLabel].first]+0.2 +0.03,IntegralCounts[0][cutLabel[iCutLabel].first] -0.04*yOffsetFactor);
	  TLine *ptLine = new TLine(IntegralCounts[1][cutLabel[iCutLabel].first],IntegralCounts[0][cutLabel[iCutLabel].first],pt->GetX1(),pt->GetY2());
	  ptLine->Draw();
	  pt->SetFillColor(0); pt->SetLineColor(1); pt->SetBorderSize(1);
	  pt->AddText(cutLabel[iCutLabel].second);
	  pt->Draw();	  
	} gPad->Update();
	TLine *vertLine = new TLine(1,gPad->GetUymin(),1,1); vertLine->SetLineColor(kBlue); vertLine->Draw();
	TLine *horizLine = new TLine(gPad->GetUxmin(),1,1,1); horizLine->SetLineColor(kBlue); horizLine->Draw();
	TLine *diagLine = new TLine(TMath::Max(gPad->GetUxmin(),gPad->GetUymin()),TMath::Max(gPad->GetUxmin(),gPad->GetUymin()),1,1); diagLine->SetLineColor(kRed); diagLine->Draw();
	
	//gPad->Update(); gPad->Modified(); gPad->Update(); gPad->Modified();
	//gPad->Print( TString::Format("plots/%s/ROC/ROC_"+nameROC_TH1F[iVar].first, dir.Data()), "png" );
	//gPad->Print( TString::Format("plots/%s/ROC/ROC_"+nameROC_TH1F[iVar].first, dir.Data()), "pdf" );
	var_C->SaveAs( TString::Format("plots/%s/ROC/"+nameROC_TH1F[iVar].first+"_distributions.png", dir.Data()) );
	
      } else Warning("Error","Histogram \""+ nameSb +"\" not found in file "+ bkg_file->GetName());
    } else Warning("Error", "Histogram \""+ nameSig +"\" not found in file "+ sig_file->GetName());
  } // for (Int_t i = 0; i < abs(nameROC_TH1F.size()); ++i)
  
}

//rsync -vut --existing roc.C cristella@cmssusy.ba.infn.it:/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/sanjay/selector/roc.C
