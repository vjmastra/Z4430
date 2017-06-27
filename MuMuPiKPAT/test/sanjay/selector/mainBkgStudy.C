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
//
// per eseguire: 
// .L mainbkgstudy.C 
// mainbkgstudy("9jan","png")
//
//////////////////////////////////////////////////
//                                              //
// THIS IS FOR PLOTS AFTER A LOOSE PRESELECTION //
// and INCLUDES THE WHOLE SPECTRUM              //
//                                              //
//////////////////////////////////////////////////
//
//
void mainBkgStudy() {
  TString extens = ".png";
  TString dir = "./plots/data/JPsi/reflections/";
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
  TString fileName = "./Data_JPsi_2p0Sig_6p0to9p0SB.root";
  //fileName = "./Data_JPsi_2p0Sig_6p0to9p0SB_dsABCD.root";
  //fileName = "./TMVA/TMVApp_data_withBDTCutAt0p00_JPsi_2p0Sig_6p0to9p0SB.root";
  fileName = "./TMVA/TMVApp_data_withBDTCutAt0p05_JPsi_2p0Sig_6p0to9p0SB.root";
  TFile f1(fileName,"read");


  // Reflcetions
  //
  // ALL SPECTRA
  /*
  TH1D *h_MassPiK, *h_MassKK, *h_MassPiPi;
  // - TH1D *hMassPK;
  TString histo1DNames[] = {"myPiKMass","myKKMass","myPiPiMass"};
  h_MassPiK = (TH1D*)f1.Get(histo1DNames[0]); h_MassPiK->SetTitle("");
  h_MassKK = (TH1D*)f1.Get(histo1DNames[1]); h_MassKK->SetTitle(""); 
  h_MassPiPi = (TH1D*)f1.Get(histo1DNames[2]); h_MassPiPi->SetTitle(""); 
  //
  //////////////////////
  //
  // SIDEBANDS SPECTRA
  *//*
  TH1D *h_MassKK_LeftSB, *h_MassKK_RightSB;
  TH1D *h_MassPiPi_LeftSB, *h_MassPiPi_RightSB;
  TH1D *h_MassPiK_LeftSB, *h_MassPiK_RightSB;
  //
  h_MassKK_LeftSB = (TH1D*)f1.Get("myKKMass_leftSb");
  h_MassKK_RightSB = (TH1D*)f1.Get("myKKMass_rightSb");
  h_MassPiPi_LeftSB = (TH1D*)f1.Get("myPiPiMass_leftSb");
  h_MassPiPi_RightSB = (TH1D*)f1.Get("myPiPiMass_rightSb");
  h_MassPiK_LeftSB = (TH1D*)f1.Get("myPiKMass_leftSb");
  h_MassPiK_RightSB = (TH1D*)f1.Get("myPiKMass_rightSb");
  //
  gStyle->SetOptStat("oe");
  //
  myC->Divide(2,2);
  myC->cd(1);
  h_MassPiK->Draw("");
  myC->cd(2);
  h_MassKK->Draw("");
  myC->cd(3);
  h_MassPiPi->Draw("");
  myC->cd(4);
  //
  myC->SaveAs(dir+"ALL_masses"+extens);
  //
  //myC->Update();  
  myC->Clear();
  */
  //
  //////////////////////
  //
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
  vector< pair< pair<TString,Bool_t>, pair< pair<Float_t,Float_t>, pair<Float_t,Float_t> > > > histoRange;
  histoRange.push_back( make_pair( make_pair("myKKMass_vs_PiKMass",kTRUE), make_pair(make_pair(KK_min,KK_max),make_pair(PiK_min,PiK_max)) ) );
  histoRange.push_back( make_pair( make_pair("myPiPiMass_vs_PiKMass",kTRUE), make_pair(make_pair(PiPi_min,PiPi_max),make_pair(PiK_min,PiK_max)) ) );
  histoRange.push_back( make_pair( make_pair("myPiPiMass_vs_KKMass",kTRUE), make_pair(make_pair(PiPi_min,PiPi_max),make_pair(KK_min,KK_max)) ) );
  histoRange.push_back( make_pair( make_pair("myPKMass_vs_PiKMass",kTRUE), make_pair(make_pair(1.3,2.3),make_pair(PiK_min,PiK_max)) ) );
  histoRange.push_back( make_pair( make_pair("myPiPMass_vs_PiKMass",kTRUE), make_pair(make_pair(1,2),make_pair(PiK_min,PiK_max)) ) );
  histoRange.push_back( make_pair( make_pair("myPsiPPKMass_vs_PsiPPiKMass",kFALSE), make_pair(make_pair(JpsiPiK_min,JpsiPiK_max),make_pair(JpsiPK_min,JpsiPK_max)) ) );

  UInt_t nHist = histoRange.size();

  vector< vector <Float_t> > xLines(nHist), yLines(nHist), xLines2D(nHist), yLines2D(nHist);
  //
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

  Int_t offset = 1000;

  TString version[] = {"","_aV",};
  Int_t nVersions = sizeof(version)/sizeof(version[0]);

  Float_t maximum = 1600;
  Bool_t tmva = kFALSE;
  if (fileName.Contains("TMVA")) {
    tmva = kTRUE;
    maximum /= 100; offset /= 50;
  }

  for (UInt_t i=0; i<nHist; ++i)
  //for (UInt_t i=nHist-1; i<nHist; ++i)
    //for (Int_t j=0; j<nVersions; ++j) {
    for (Int_t j=1; j<nVersions; ++j) {

      TString histoName = histoRange[i].first.first + version[j];
      if (tmva) {
	histoName = histoRange[i].first.first + "_aV_BDT_1B0";
	j = nVersions; // so that it will not loop again
      }
      TH2F* th2 = (TH2F*)f1.Get(histoName);
      if (!th2) {
	cout <<"WARNING! Histogram \"" <<histoName <<"\" not found in TFile \"" <<fileName <<"\"" <<endl;
	continue; }

      th2->GetXaxis()->SetRangeUser(histoRange[i].second.first.first, histoRange[i].second.first.second);
      th2->GetYaxis()->SetRangeUser(histoRange[i].second.second.first, histoRange[i].second.second.second);
      th2->SetTitle("");
      TH1F* xProj = (TH1F*)th2->ProjectionX();
      TH1F* yProj = (TH1F*)th2->ProjectionY();

      myC->Divide(2,2);

      // Y projection
      myC->cd(1);
      yProj->Draw("hbar");
      //
      if (histoRange[i].first.second) {
	gPad->Update();
        for (UInt_t yLine = 0; yLine < yLines[i].size(); ++yLine) {
	  Float_t yVal = yLines[i][yLine];
	  TLine* temp = new TLine(offset + yProj->GetBinContent(yProj->FindBin(yVal)), yVal, gPad->GetX2(), yVal);
	  temp->SetLineStyle(2); temp->SetLineWidth(3); temp->SetLineColor(kRed); temp->Draw("same");
        }
      }
      //
      // to set specific stat box for only this plot (gStyle dominates otherwise)
      gPad->Update();
      TPaveStats *psY = (TPaveStats*)yProj->GetListOfFunctions()->FindObject("stats");
      psY->SetOptStat(10);  //psY->SetOptStat(110010); ith rotation both OVFW & UNFW are put to 0 !!!!
      psY->SetX1NDC(0.7); psY->SetX2NDC(0.9); psY->SetY1NDC(0.85); psY->SetY2NDC(0.9);
      gPad->Modified();
      gPad->Update();


      // scatter plot
      myC->cd(2);
      th2->SetStats(0);
      th2->SetMinimum(10.); //th2->SetMaximum(maximum);
      //h2_MassPiPi_vs_MassPiK->SetMaximum(1000.); // 1200 good
      th2->Draw("COLZ0");
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
      TPaletteAxis *palette = (TPaletteAxis*)th2->GetListOfFunctions()->FindObject("palette");
      palette->SetX1NDC(0.85); palette->SetX2NDC(0.9); palette->SetY1NDC(0.1); palette->SetY2NDC(0.7);
      //gPad->Modified(); //gPad->Update();


      // X projection
      myC->cd(4); 
      xProj->Draw("");
      //
      if (histoRange[i].first.second) {
	gPad->Update();
        for (UInt_t xLine = 0; xLine < xLines[i].size(); ++xLine) {
	  Float_t xVal = xLines[i][xLine];
	  TLine* temp = new TLine(xVal, offset + xProj->GetBinContent(xProj->FindBin(xVal)), xVal, gPad->GetY2());
	  temp->SetLineStyle(2); temp->SetLineWidth(3); temp->SetLineColor(kRed); temp->Draw("same");
        }
      }
      //
      gPad->Update();
      TPaveStats *psX = (TPaveStats*)xProj->GetListOfFunctions()->FindObject("stats");
      psX->SetX1NDC(0.7); psX->SetX2NDC(0.9); psX->SetY1NDC(0.85); psX->SetY2NDC(0.9);
      psX->SetOptStat(10);
      //gPad->Modified(); //gPad->Update();


      myC->SaveAs(dir + th2->GetName() + extens);
      //myC->Update();  
      myC->Clear();
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

