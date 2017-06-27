#include <TROOT.h>
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TPaveStats.h>
#include <TPaletteAxis.h>
#include <string>
#include <iostream>
#include <fstream>

#include "/lustrehome/cristella/work/Z_analysis/AA_fit/constants.h"
#include "dalitzContour.C"
//
// per eseguire: 
// .L scatterPlot.C 
// scatterPlot()
//
//////////////////////////////////////////////////
//                                              //
// THIS IS FOR PLOTS AFTER A LOOSE PRESELECTION //
// and INCLUDES THE WHOLE SPECTRUM              //
//                                              //
//////////////////////////////////////////////////
//
//
void scatterPlot() {
  TString extens = ".png";
  TString dir = "./TMVA/plots/data/JPsi/";
  //
  gROOT->Reset();
  gROOT->Clear();
  //
  gROOT->SetStyle("Plain");
  TGaxis::SetMaxDigits(3) ;
  //
  TCanvas *myC = new TCanvas("myC","Plots",1000,800);
  //
  myC->SetFrameFillColor(0);
  myC->SetBorderSize(2);
  myC->SetGridx(0); myC->SetGridy(0);
  //
  myC->SetRightMargin(0.); myC->SetLeftMargin(0.); myC->SetTopMargin(0.); myC->SetBottomMargin(0.);
  //
  myC->cd();
  //
  TString training = "2p0Sig_6p0to9p0SB";
  TString fileName = "./Data_JPsi_"+training+".root";
  //fileName = "./Data_JPsi_"+training+"_dsABCD.root";
  // keep the following number always syncronised with the one shown in TMVA/b_peak/plots/JPsi/+training+/vars_BDTCutAt+bdtCut+/myMuMuKPiMassAltZoom_BDTCutAt0p00_1B0.png
  TString bdtCut = "0p00"; Float_t purity = 0.752;
  bdtCut = "0p05"; purity = 0.775;

  TString selection = "withBDTCutAt"+bdtCut;

  fileName = "./TMVA/TMVApp_data_"+selection+"_JPsi_"+training+".root";

  Bool_t tmva = kTRUE;
  if (!fileName.Contains("TMVA")) {
    tmva = kFALSE;
    selection = "hardCuts";
    purity = 0.815; }
  TFile f1(fileName,"read");

  dir.Append(training+"/vars_BDTCutAt"+bdtCut+"/");

  // SCATTER PLOTS
  //
  //Int_t MyPalette[100];
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Int_t MyPalette[NCont];
  //Double_t red[] = {1.0, 0.0, 1.0, 1.0};
  //Double_t green[] = {1.0, 0.0, 0.0, 1.0 };
  //Double_t blue[] = {1.0, 1.0, 0.0, 0.0};
  //Double_t stop[] = {0.0, 0.5, 0.9, 1.0};
  ///Int_t FI = TColor::CreateGradientColorTable(4, stop, red, green, blue, 100);
  Double_t stop[NRGBs] = {0.00, 0.34, 0.61, 0.84, 1.00};
  Double_t red[NRGBs] = {0.00, 0.00, 0.87, 1.00, 0.51};
  Double_t green[NRGBs] = {0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs] = {0.51, 1.00, 0.12, 0.00, 0.00};
  Int_t FI = TColor::CreateGradientColorTable(NRGBs, stop, red, green, blue, NCont);
  //for (int i=0; i<100; i++) MyPalette[i] = FI+i;
  //gStyle->SetPalette(100, MyPalette);
  for (int i=0; i<255; i++) MyPalette[i] = FI+i;
  gStyle->SetPalette(NCont, MyPalette);
  gStyle->SetNumberContours(NCont);
  //
    
  Float_t PiK_min = 0.55, PiK_max = 1.65, KK_min = 0.9, KK_max = 1.9, PiPi_min = 0.2, PiPi_max = 1.45, JpsiPiK_min = 5, JpsiPiK_max = 5.6, JpsiPK_min = 5.2, JpsiPK_max = 5.9;
  Float_t PiK2_min = 0., PiK2_max = 5., JpsiPi2_min = 9., JpsiPi2_max = 25.;
  vector< pair< pair<TString,Bool_t>, pair< pair<Float_t,Float_t>, pair<Float_t,Float_t> > > > histoRange;
  histoRange.push_back( make_pair( make_pair("psi2SPi_vs_KPi_Dalitz",kFALSE), make_pair(make_pair(PiK2_min,PiK2_max),make_pair(JpsiPi2_min,JpsiPi2_max)) ) );

  UInt_t nHist = histoRange.size();

  vector< vector <Float_t> > xLines(nHist), yLines(nHist), xLines2D(nHist), yLines2D(nHist);
  /*
  xLines[0].push_back(0.983); xLines[0].push_back(1.02); xLines[0].push_back(1.09);
  yLines[0].push_back(0.72); yLines[0].push_back(0.815); yLines[0].push_back(0.895);
  yLines2D[0].push_back(0.96); yLines2D[0].push_back(0.98); yLines2D[0].push_back(1.02);
  xLines2D[0].push_back(0.65); xLines2D[0].push_back(0.6); xLines2D[0].push_back(0.6);
  //
  xLines[1].push_back(0.275); xLines[1].push_back(0.37); xLines[1].push_back(0.71);
  yLines[1].push_back(0.72); yLines[1].push_back(0.815); yLines[1].push_back(0.895);
  yLines2D[1].push_back(0.25); yLines2D[1].push_back(0.25); yLines2D[1].push_back(0.25);
  xLines2D[1].push_back(0.65); xLines2D[1].push_back(0.6); xLines2D[1].push_back(0.8);
  //
  xLines[2].push_back(0.275); xLines[2].push_back(0.37); xLines[2].push_back(0.71);
  yLines[2].push_back(0.984); yLines[2].push_back(1.02); yLines[2].push_back(1.09);
  yLines2D[2].push_back(0.23); yLines2D[2].push_back(0.25); yLines2D[2].push_back(0.28);
  xLines2D[2].push_back(0.95); xLines2D[2].push_back(0.98); xLines2D[2].push_back(1.16);
  //
  xLines[3].push_back(1.49); xLines[3].push_back(1.56);
  yLines[3].push_back(0.72); yLines[3].push_back(0.815); yLines[3].push_back(0.895);
  yLines2D[3].push_back(1.4); yLines2D[3].push_back(1.4); yLines2D[3].push_back(1.4);
  xLines2D[3].push_back(0.65); xLines2D[3].push_back(0.63);
  //
  xLines[4].push_back(1.32);
  yLines[4].push_back(0.72); yLines[4].push_back(0.815); yLines[4].push_back(0.895);
  yLines2D[4].push_back(1.04); yLines2D[4].push_back(1.08); yLines2D[4].push_back(1.14);
  xLines2D[4].push_back(0.65);
  */
  Int_t offset = 1000;

  TString version[] = {"_1B0_signalWin"};
  Int_t nVersions = sizeof(version)/sizeof(version[0]);

  Float_t maximum = 1600;
  if (tmva) {
    maximum /= 100; offset /= 50;
  } else
    version[0].Prepend("_"+selection);

  for (UInt_t i=0; i<nHist; ++i) {
  //for (UInt_t i=nHist-1; i<nHist; ++i) {

    // Dalitz contour
    pair<TGraph*,TGraph*> dalCont = dalitzContour(MBd, MKaon, MPion, MJpsi, 18000);
    if (!dalCont.first || !dalCont.second)
      cout <<"\nWARNING! No Dalitz contour created!" <<endl;
    else {
      dalCont.first->SetLineWidth(3); dalCont.second->SetLineWidth(3);
    }
    cout <<"\nUsing " <<histoRange[i].first.first <<" histograms" <<endl;

    for (Int_t j=0; j<nVersions; ++j) {
    //for (Int_t j=1; j<nVersions; ++j) {
      cout <<"in version \"" <<version[j] <<"\"" <<endl;

      TString histoSigName = histoRange[i].first.first + version[j];
      TString histoSbName = histoRange[i].first.first + "_sbs";
      if (tmva) {
	histoSigName.Append("_BDT");
	histoSbName.Append("_BDT");
      } else {
	histoSbName.ReplaceAll("sbs",selection+"_1B0_sidebands");
        histoSigName.Append("_B0massC"); histoSbName.Append("_B0massC"); selection.Append("_B0massC");
      }

      TH2F* th2Sig = (TH2F*)f1.Get(histoSigName);
      if (!th2Sig) {
	cout <<"WARNING! Histogram \"" <<histoSigName <<"\" not found in TFile \"" <<fileName <<"\"" <<endl;
	continue; }

      TH2F* th2Sb = (TH2F*)f1.Get(histoSbName);
      if (!th2Sb) {
	cout <<"WARNING! Histogram \"" <<histoSbName <<"\" not found in TFile \"" <<fileName <<"\"" <<endl;
	continue; }
      else if (th2Sig) {
	cout <<"\nScaling sidebands to signal region" <<endl;
	th2Sb->Scale( th2Sig->Integral()/th2Sb->Integral() * (1-purity) );
      }

      //th2Sig->GetXaxis()->SetRangeUser(histoRange[i].second.first.first, histoRange[i].second.first.second);
      //th2Sig->GetYaxis()->SetRangeUser(histoRange[i].second.second.first, histoRange[i].second.second.second);
      th2Sig->SetTitle(""); th2Sb->SetTitle("");
      TH1F* xProjSig = (TH1F*)th2Sig->ProjectionX();
      TH1F* yProjSig = (TH1F*)th2Sig->ProjectionY();
      TH1F* xProjSb = (TH1F*)th2Sb->ProjectionX(); xProjSb->SetLineColor(kRed);
      TH1F* yProjSb = (TH1F*)th2Sb->ProjectionY(); yProjSb->SetLineColor(kRed);

      myC->Divide(2,2);

      // Y projection
      myC->cd(1);
      TH1F* mainY = 0;
      if (yProjSig->GetMaximum() > yProjSb->GetMaximum()) {
	mainY = yProjSig;
	yProjSig->Draw(); yProjSb->Draw("same"); }
      else {
	mainY = yProjSb;
	yProjSb->Draw(); yProjSig->Draw("same"); }
      //
      if (histoRange[i].first.second) {
	gPad->Update();
        for (UInt_t yLine = 0; yLine < yLines[i].size(); ++yLine) {
	  Float_t yVal = yLines[i][yLine];
	  TLine* temp = new TLine(offset + mainY->GetBinContent(mainY->FindBin(yVal)), yVal, gPad->GetX2(), yVal);
	  temp->SetLineStyle(2); temp->SetLineWidth(3); temp->SetLineColor(kRed); temp->Draw("same");
        }
      }
      //
      // to set specific stat box for only this plot (gStyle dominates otherwise)
      gPad->Update();
      TPaveStats *psY = (TPaveStats*)mainY->GetListOfFunctions()->FindObject("stats");
      psY->SetOptStat(10);  //psY->SetOptStat(110010); ith rotation both OVFW & UNFW are put to 0 !!!!
      psY->SetX1NDC(0.7); psY->SetX2NDC(0.9); psY->SetY1NDC(0.85); psY->SetY2NDC(0.9);
      gPad->Modified();
      gPad->Update();


      // scatter plot signal
      myC->cd(2);
      th2Sig->SetStats(0);
      //th2Sig->SetMinimum(10.); //th2Sig->SetMaximum(maximum);
      th2Sig->Draw("COLZ0");
      dalCont.first->Draw("lsame"); dalCont.second->Draw("lsame");
      //
      if (histoRange[i].first.second) {
	gPad->Update();
        for (UInt_t yLine = 0; yLine < yLines2D[i].size(); ++yLine) {
	  Float_t yVal = yLines[i][yLine];
	  TLine* temp = new TLine(gPad->GetX1(), yVal, yLines2D[i][yLine], yVal);
	  temp->SetLineStyle(2); temp->SetLineWidth(3); temp->SetLineColor(kRed); temp->Draw("same");
        }
        for (UInt_t xLine = 0; xLine < xLines2D[i].size(); ++xLine) {
	  Float_t xVal = xLines[i][xLine];
	  Float_t yVal = xLines2D[i][xLine];
	  TLine* temp = new TLine(xVal, gPad->GetY1(), xVal, xLines2D[i][xLine]);
	  temp->SetLineStyle(2); temp->SetLineWidth(3); temp->SetLineColor(kRed); temp->Draw("same");
        }
	if (i==0 || i==1 || i==2) {
	  TLine* extra = new TLine(xLines[i][xLines2D[i].size()-1], yLines[i][yLines2D[i].size()-1], xLines[i][xLines2D[i].size()-1], yLines[i][yLines2D[i].size()-1]);
	  if ( i==0 ) { // "myKKMass_vs_PiKMass"
	    extra->SetY1(1) ; extra->SetY2(1.4);
	  } else if ( i==1 ) { // "myPiPiMass_vs_PiKMass"
	    extra->SetX1(0.8); extra->SetX2(1.0);
	  } else if ( i==2 ) { // "myPiPiMass_vs_KKMass"
	    extra->SetX1(0.6); extra->SetX2(0.9);
	  }
	  extra->SetLineStyle(2); extra->SetLineWidth(3); extra->SetLineColor(kRed); extra->Draw("same");
	  extra->Draw("same");
	}
      }
      //
      gPad->Update();
      TPaletteAxis *paletteSig = (TPaletteAxis*)th2Sig->GetListOfFunctions()->FindObject("palette");
      if (paletteSig) {
	paletteSig->SetX1NDC(0.9); paletteSig->SetX2NDC(0.95); paletteSig->SetY1NDC(0.1); paletteSig->SetY2NDC(0.9);
      //gPad->Modified(); //gPad->Update();
      }

      // X projection
      myC->cd(4);
      TH1F* mainX = 0;
      if (xProjSig->GetMaximum() > xProjSb->GetMaximum()) {
	mainX = xProjSig;
	xProjSig->Draw(); xProjSb->Draw("same"); }
      else {
	mainX = xProjSb;
	xProjSb->Draw(); xProjSig->Draw("same"); }
      //
      if (histoRange[i].first.second) {
	gPad->Update();
        for (UInt_t xLine = 0; xLine < xLines[i].size(); ++xLine) {
	  Float_t xVal = xLines[i][xLine];
	  TLine* temp = new TLine(xVal, offset + mainX->GetBinContent(mainX->FindBin(xVal)), xVal, gPad->GetY2());
	  temp->SetLineStyle(2); temp->SetLineWidth(3); temp->SetLineColor(kRed); temp->Draw("same");
        }
      }
      //
      gPad->Update();
      TPaveStats *psX = (TPaveStats*)mainX->GetListOfFunctions()->FindObject("stats");
      psX->SetOptStat(10);
      psX->SetX1NDC(0.7); psX->SetX2NDC(0.9); psX->SetY1NDC(0.85); psX->SetY2NDC(0.9);
      gPad->Modified(); //gPad->Update();


      // scatter plot sidebands
      myC->cd(3);
      th2Sb->SetStats(0);
      //th2Sb->SetMinimum(10.); //th2Sb->SetMaximum(maximum);
      th2Sb->Draw("COLZ0");
      dalCont.first->Draw("lsame"); dalCont.second->Draw("lsame");
      //
      if (histoRange[i].first.second) {
	gPad->Update();
        for (UInt_t yLine = 0; yLine < yLines2D[i].size(); ++yLine) {
	  Float_t yVal = yLines[i][yLine];
	  TLine* temp = new TLine(gPad->GetX1(), yVal, yLines2D[i][yLine], yVal);
	  temp->SetLineStyle(2); temp->SetLineWidth(3); temp->SetLineColor(kRed); temp->Draw("same");
        }
        for (UInt_t xLine = 0; xLine < xLines2D[i].size(); ++xLine) {
	  Float_t xVal = xLines[i][xLine];
	  Float_t yVal = xLines2D[i][xLine];
	  TLine* temp = new TLine(xVal, gPad->GetY1(), xVal, xLines2D[i][xLine]);
	  temp->SetLineStyle(2); temp->SetLineWidth(3); temp->SetLineColor(kRed); temp->Draw("same");
        }
	if (i==0 || i==1 || i==2) {
	  TLine* extra = new TLine(xLines[i][xLines2D[i].size()-1], yLines[i][yLines2D[i].size()-1], xLines[i][xLines2D[i].size()-1], yLines[i][yLines2D[i].size()-1]);
	  if ( i==0 ) { // "myKKMass_vs_PiKMass"
	    extra->SetY1(1) ; extra->SetY2(1.4);
	  } else if ( i==1 ) { // "myPiPiMass_vs_PiKMass"
	    extra->SetX1(0.8); extra->SetX2(1.0);
	  } else if ( i==2 ) { // "myPiPiMass_vs_KKMass"
	    extra->SetX1(0.6); extra->SetX2(0.9);
	  }
	  extra->SetLineStyle(2); extra->SetLineWidth(3); extra->SetLineColor(kRed); extra->Draw("same");
	  extra->Draw("same");
	}
      }
      //
      gPad->Update();
      TPaletteAxis *paletteSb = (TPaletteAxis*)th2Sb->GetListOfFunctions()->FindObject("palette");
      if (paletteSb) {
	paletteSb->SetX1NDC(0.9); paletteSb->SetX2NDC(0.95); paletteSb->SetY1NDC(0.1); paletteSb->SetY2NDC(0.9);
      }

      myC->SaveAs(dir + histoRange[i].first.first +"_"+ selection + extens);
      //myC->Update();  
      myC->Clear();
    }
  }
  ///////////////////////////
  /*
  // Sidebands
  gStyle->SetOptStat("e");

  Float_t PiK_min = 0.55, PiK_max = 1.65, KK_min = 0.9, KK_max = 1.9, PiPi_min = 0.2, PiPi_max = 1.45;

  vector< pair< TString, pair< pair<Float_t,Float_t>, pair<Float_t,Float_t> > > > scatterName;
  scatterName.push_back( make_pair("myPiPiMass_vs_PiKMass", make_pair( make_pair(PiPi_min,PiPi_max), make_pair(PiK_min,PiK_max) ) ) );
  scatterName.push_back( make_pair("myKKMass_vs_PiKMass", make_pair( make_pair(KK_min,KK_max), make_pair(PiK_min,PiK_max) ) ) );
  TString sbName[] = {"fromB0peak","rightSb","leftSb"};
  UInt_t nSb = sizeof(sbName)/sizeof(sbName[0]);

  for (UInt_t iScatt=0; iScatt < scatterName.size(); ++iScatt) {
    myC->Divide(2,2);
    for (UInt_t iSb=0; iSb < nSb; ++iSb) {
      TString histoName = scatterName[iScatt].first + "_" + sbName[iSb];
      TH2F* temp = (TH2F*)f1.Get(histoName);
      if (!temp) {
	cout <<"WARNING! Histogram \"" <<histoName <<"\" not found in TFile \"" <<fileName <<"\"" <<endl;
	continue; }
      
      temp->GetXaxis()->SetRangeUser(scatterName[iScatt].second.first.first, scatterName[iScatt].second.first.second);
      temp->GetYaxis()->SetRangeUser(scatterName[iScatt].second.second.first, scatterName[iScatt].second.second.second);
      temp->SetTitle("");
      
      myC->cd(iSb+1);
      temp->Draw("colz");
    }
    myC->SaveAs(dir + scatterName[iScatt].first + extens);
    myC->Clear();
  }
  */
  ////////////////////
  delete myC;
  //
  f1.Close();
  f1.Delete();
  //
  gROOT->Reset();
  gROOT->Clear();
  //
}

