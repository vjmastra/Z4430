#include <cstdlib>
#include <iostream> 
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

// For the new ROOT version (6.06.08)
#include "TMVA/Factory.h"
//#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

void TMVAClassification_old( ) { 
  
   TString psi_nS = "psi2S"; psi_nS = "JPsi";

   TString mumu_label = "#psi'" ;
   if (psi_nS.EqualTo("JPsi"))
     mumu_label = "J/#psi" ;

   Float_t B0_signal, innerSB, outerSB;
   B0_signal = 2.;
   //innerSB = 5. ; outerSB = 9.;
   //innerSB = 4. ; outerSB = 6.;
   innerSB = 6. ; outerSB = 9.;
   TString B0limits = TString::Format("%.1fSig_%.1fto%.1fSB",B0_signal,innerSB,outerSB);
   B0limits.ReplaceAll(".","p") ;
   //TFile *inputSig = TFile::Open("../officialMC4tmva_"+psi_nS+"_tightCuts_"+"2p0Sig_4p0to6p0SB"+".root");
   TFile *inputSig = 0;

   // Create a new root output file.
   TString outfileName = "TMVA_"+psi_nS+"_"+B0limits+".root";
   //outfileName = "TMVA_28M.root";

   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   TMVA::Factory *factory = new TMVA::Factory( psi_nS+"/TMVA", outputFile, "!V:!Silent");


   //factory->AddVariable( "pionPt",             "p_{T}(#pi^{-})",                              "GeV", 'F' );
   //factory->AddVariable( "kaonPt",             "p_{T}(K^{+})",                                "GeV", 'F' );
   factory->AddVariable( "higherPtTrk",          "higher p_{T} track",                          "GeV", 'F' );
   factory->AddVariable( "lowerPtTrk",           "lower p_{T} track",                           "GeV", 'F' );
   factory->AddVariable( "pionPtPlusKaonPt",     "p_{T}(#pi^{-}) + p_{T}(K^{+})",               "GeV", 'F' );
   factory->AddVariable( "psiDeltaRHigherPtTrk", "#DeltaR("+mumu_label+", higher p_{T} track)", "",    'F' );
   factory->AddVariable( "psiDeltaRLowerPtTrk",  "#DeltaR("+mumu_label+", lower p_{T} track)",  "",    'F' );
   factory->AddVariable( "psiDeltaRKPi",         "#DeltaR("+mumu_label+", #pi^{-}K^{+})",       "",    'F' );
   factory->AddVariable( "pionDeltaRKaon",       "#DeltaR(#pi^{-},K^{+})",                      "",    'F' );
   //factory->AddVariable( "KPi_mass",           "KPi_mass",             "GeV", 'F' );
   factory->AddVariable( "B0Pt",                 "p_{T}(B^{0})",                                "GeV", 'F' );
   factory->AddVariable( "B0VtxCL",              "B^{0} vertex CL",                             "",    'F' );
   factory->AddVariable( "B0_PVCTau",            "B^{0} flight lenght significance",            "",    'F' );
   //factory->AddVariable( "B0_BSCTau",          "B0_BSCTau",            "",    'F' );
   factory->AddVariable( "B0_pointingAnglePV",   "B^{0} pointing angle",                        "",    'F' );


   // read training and test data
   // load the signal and background event samples from ROOT trees

   TString fileNameSig, fileNameBkg;
   //fileNameSig = "TightCuts_histos_officialMC.root"; fileNameBkg = "Data_tightCuts.root";
   if (psi_nS.EqualTo("psi2S"))
     fileNameSig = "officialMC4tmva_"+psi_nS+"_Bd2MuMuKPi_"+B0limits+".root";
   else
     fileNameSig = "officialMC_noPtEtaCuts_"+psi_nS+"_Bd2MuMuKPi_"+B0limits+".root";

   fileNameBkg = "Data_"+psi_nS+"_"+B0limits+".root";
   //TFile *input = TFile::Open( "MonteCarlo/MonteCarloEvents8D.root"); // check if file in local directory exists
   inputSig = TFile::Open( "../"+fileNameSig );
   TFile *inputBkg = TFile::Open( "../"+fileNameBkg );

   TString sigTreeName = "mva_variables_sig";
   TString bkgTreeName = "mva_variables_bkg";
   if (!inputSig) return;
   if (!inputBkg) return;
   TTree *signal     = (TTree*)inputSig->Get( sigTreeName );
   TTree *background = (TTree*)inputBkg->Get( bkgTreeName );

   if (signal) {
     factory->AddSignalTree( signal );
     if (background) {
       factory->AddBackgroundTree( background );
       } else { cout <<"Background tree \"" <<bkgTreeName <<"\" not found in file \"" <<fileNameBkg <<"\". Pleaes check" <<endl;
	 return; }
   } else { cout <<"Signal tree \"" <<sigTreeName <<"\" not found in file \"" <<fileNameSig <<"\". Please check" <<endl;
     return;
   }

   // tell the factory to use all remaining events in the trees after training for testing:
   factory->PrepareTrainingAndTestTree( "","", "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

   // need to choose at least one method
   //factory->BookMethod( TMVA::Types::kLD, "LD", "H:V:" );
   //factory->BookMethod( TMVA::Types::kLikelihood, "Likelihood", "H:V:" );
   factory->BookMethod( TMVA::Types::kBDT, "BDTDefault", "H:V");
   //factory->BookMethod( TMVA::Types::kMLP, "MLPDefault", "H:V:" );
   //factory->BookMethod( TMVA::Types::kBDT, "BDTDG", "H:V");

   // Train MVAs using the set of training events
   factory->TrainAllMethods();

   // ---- Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // ----- Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();    

   // --------------------------------------------------------------
   
   // Save the output
   outputFile->Close();

   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) {
     if (gROOT->GetVersionInt() > 60402) TMVA::TMVAGui( outfileName ); 
     else                                {gROOT->ProcessLine(".L TMVAGui.C"); TMVA::TMVAGui( outfileName );}
   } 
   // button to press: 1a, 3, 4b, 5a, 5b, 
   delete factory;
}
