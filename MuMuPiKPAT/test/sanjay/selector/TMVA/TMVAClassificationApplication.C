/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVAClassificationApplication                                      *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

// in order to run way faster compile first with +:
// root -l
// .L TMVAClassificationApplication.C+ // the '+' is important to speed up!
// TMVAClassificationApplication()

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TH2.h"
#include "TNtupleD.h"
#include "TMath.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TStyle.h"
#include "TString.h"

//#include "TMVAGui.C"
//#include "TMVA/TMVAGui.h"

//#include </lustrehome/cristella/work/Z_analysis/AA_fit/utilities.h>
#include </lustrehome/cristella/work/Z_analysis/AA_fit/myPDF.h>

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

using namespace TMVA;

void TMVAClassificationApplication( TString myMethodList = "" ) 
{   
#ifdef __CINT__
   gROOT->ProcessLine( ".O0" ); // turn off optimization in CINT
#endif

   Double_t B0_mass = 5.27958;
   Double_t jpsi_mass = 3.0967;
   Double_t psi2S_mass = 3.686109;
   Double_t kaonCh_mass = 0.493677;
   Double_t pionCh_mass = 0.13957018;
   Double_t kaonCh_mass2 = kaonCh_mass*kaonCh_mass;
   Double_t pionCh_mass2 = pionCh_mass*pionCh_mass;
   //---------------------------------------------------------------

   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   /*
   // --- Cut optimisation
   Use["Cuts"]            = 1;
   Use["CutsD"]           = 1;
   Use["CutsPCA"]         = 0;
   Use["CutsGA"]          = 0;
   Use["CutsSA"]          = 0;
   // 
   // --- 1-dimensional likelihood ("naive Bayes estimator")
   Use["Likelihood"]      = 1;
   Use["LikelihoodD"]     = 0; // the "D" extension indicates decorrelated input variables (see option strings)
   Use["LikelihoodPCA"]   = 1; // the "PCA" extension indicates PCA-transformed input variables (see option strings)
   Use["LikelihoodKDE"]   = 0;
   Use["LikelihoodMIX"]   = 0;
   //
   // --- Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 1;
   Use["PDERSD"]          = 0;
   Use["PDERSPCA"]        = 0;
   Use["PDEFoam"]         = 1;
   Use["PDEFoamBoost"]    = 0; // uses generalised MVA method boosting
   Use["KNN"]             = 1; // k-nearest neighbour method
   //
   // --- Linear Discriminant Analysis
   Use["LD"]              = 1; // Linear Discriminant identical to Fisher
   Use["Fisher"]          = 0;
   Use["FisherG"]         = 0;
   Use["BoostedFisher"]   = 0; // uses generalised MVA method boosting
   Use["HMatrix"]         = 0;
   //
   // --- Function Discriminant analysis
   Use["FDA_GA"]          = 1; // minimisation of user-defined function using Genetics Algorithm
   Use["FDA_SA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   Use["FDA_MCMT"]        = 0;
   //
   // --- Neural Networks (all are feed-forward Multilayer Perceptrons)
   Use["MLP"]             = 0; // Recommended ANN
   Use["MLPBFGS"]         = 0; // Recommended ANN with optional training method
   Use["MLPBNN"]          = 1; // Recommended ANN with BFGS training method and bayesian regulator
   Use["CFMlpANN"]        = 0; // Depreciated ANN from ALEPH
   Use["TMlpANN"]         = 0; // ROOT's own ANN
   //
   // --- Support Vector Machine 
   Use["SVM"]             = 1;
   //
   */ 
   // --- Boosted Decision Trees
   Use["BDT"]             = 1; // uses Adaptive Boost
   Use["BDTG"]            = 0; // uses Gradient Boost
   Use["BDTB"]            = 0; // uses Bagging
   Use["BDTD"]            = 0; // decorrelation + Adaptive Boost
   //
   /* 
   // --- Friedman's RuleFit method, ie, an optimised series of cuts ("rules")
   Use["RuleFit"]         = 1;
   // ---------------------------------------------------------------
   Use["Plugin"]          = 0;
   Use["Category"]        = 0;
   Use["SVM_Gauss"]       = 0;
   Use["SVM_Poly"]        = 0;
   Use["SVM_Lin"]         = 0;
   */

   std::cout << "\n==> Start TMVAClassificationApplication" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod 
                      << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
               std::cout << it->first << " ";
            }
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Create the Reader object

   TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );    

   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
   /*
   Float_t var1, var2;
   Float_t var3, var4;
   reader->AddVariable( "myvar1 := var1+var2", &var1 );
   reader->AddVariable( "myvar2 := var1-var2", &var2 );
   reader->AddVariable( "var3",                &var3 );
   reader->AddVariable( "var4",                &var4 );
   */

   vector <TString> var_name; // remember to sync this list with the one in TNVAClassification in the same order
   var_name.push_back("higherPtTrk"); var_name.push_back("lowerPtTrk"); var_name.push_back("pionPtPlusKaonPt");
   var_name.push_back("psiDeltaRHigherPtTrk"); var_name.push_back("psiDeltaRLowerPtTrk"); var_name.push_back("psiDeltaRKPi"); var_name.push_back("pionDeltaRKaon");
   //var_name.push_back("KPi_mass");
   var_name.push_back("B0Pt"); var_name.push_back("B0VtxCL"); var_name.push_back("B0_PVCTau"); //var_name.push_back("B0_BSCTau");
   var_name.push_back("B0_pointingAnglePV");

   const Int_t BDT_vars_number = var_name.size();
   cout <<"BDT_vars_number = " <<BDT_vars_number <<endl;
   vector <Float_t> var(BDT_vars_number,0);

   for (Int_t i=0; i<BDT_vars_number; ++i)
     reader->AddVariable( var_name[i], &var[i] );
   
   // Spectator variables declared in the training have to be added to the reader, too
   /*
   Float_t spec1,spec2;
   reader->AddSpectator( "spec1 := var1*2",   &spec1 );
   reader->AddSpectator( "spec2 := var1*3",   &spec2 );
   */
   /*
   Float_t Category_cat1, Category_cat2, Category_cat3;
   if (Use["Category"]){
      // Add artificial spectators for distinguishing categories
      reader->AddSpectator( "Category_cat1 := var3<=0",             &Category_cat1 );
      reader->AddSpectator( "Category_cat2 := (var3>0)&&(var4<0)",  &Category_cat2 );
      reader->AddSpectator( "Category_cat3 := (var3>0)&&(var4>=0)", &Category_cat3 );
   }
   */
   // --- Book the MVA methods

   TString psi_nS = "psi2S"; psi_nS = "JPsi";
   TString dir    = "weights/"+psi_nS+"/";
   TString prefix = "TMVAClassification";

   // Book method(s)
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = TString(it->first) + TString(" method");
         TString weightfile = dir + prefix + TString("_") + TString(it->first) + TString(".weights.xml");
         //reader->BookMVA( methodName, weightfile );
	 reader->BookMVA( methodName, dir+"TMVA_BDTDefault.weights.xml" ); 
      }
   }
   
   // Book output histograms
   UInt_t nbin = 100;
   TH1F   *histLk(0), *histLkD(0), *histLkPCA(0), *histLkKDE(0), *histLkMIX(0), *histPD(0), *histPDD(0);
   TH1F   *histPDPCA(0), *histPDEFoam(0), *histPDEFoamErr(0), *histPDEFoamSig(0), *histKNN(0), *histHm(0);
   TH1F   *histFi(0), *histFiG(0), *histFiB(0), *histLD(0), *histNn(0),*histNnbfgs(0),*histNnbnn(0);
   TH1F   *histNnC(0), *histNnT(0), *histBdt(0), *histBdtG(0), *histBdtD(0), *histRf(0), *histSVMG(0);
   TH1F   *histSVMP(0), *histSVML(0), *histFDAMT(0), *histFDAGA(0), *histCat(0), *histPBdt(0);

   if (Use["Likelihood"])    histLk      = new TH1F( "MVA_Likelihood",    "MVA_Likelihood",    nbin, -1, 1 );
   if (Use["LikelihoodD"])   histLkD     = new TH1F( "MVA_LikelihoodD",   "MVA_LikelihoodD",   nbin, -1, 0.9999 );
   if (Use["LikelihoodPCA"]) histLkPCA   = new TH1F( "MVA_LikelihoodPCA", "MVA_LikelihoodPCA", nbin, -1, 1 );
   if (Use["LikelihoodKDE"]) histLkKDE   = new TH1F( "MVA_LikelihoodKDE", "MVA_LikelihoodKDE", nbin,  -0.00001, 0.99999 );
   if (Use["LikelihoodMIX"]) histLkMIX   = new TH1F( "MVA_LikelihoodMIX", "MVA_LikelihoodMIX", nbin,  0, 1 );
   if (Use["PDERS"])         histPD      = new TH1F( "MVA_PDERS",         "MVA_PDERS",         nbin,  0, 1 );
   if (Use["PDERSD"])        histPDD     = new TH1F( "MVA_PDERSD",        "MVA_PDERSD",        nbin,  0, 1 );
   if (Use["PDERSPCA"])      histPDPCA   = new TH1F( "MVA_PDERSPCA",      "MVA_PDERSPCA",      nbin,  0, 1 );
   if (Use["KNN"])           histKNN     = new TH1F( "MVA_KNN",           "MVA_KNN",           nbin,  0, 1 );
   if (Use["HMatrix"])       histHm      = new TH1F( "MVA_HMatrix",       "MVA_HMatrix",       nbin, -0.95, 1.55 );
   if (Use["Fisher"])        histFi      = new TH1F( "MVA_Fisher",        "MVA_Fisher",        nbin, -4, 4 );
   if (Use["FisherG"])       histFiG     = new TH1F( "MVA_FisherG",       "MVA_FisherG",       nbin, -1, 1 );
   if (Use["BoostedFisher"]) histFiB     = new TH1F( "MVA_BoostedFisher", "MVA_BoostedFisher", nbin, -2, 2 );
   if (Use["LD"])            histLD      = new TH1F( "MVA_LD",            "MVA_LD",            nbin, -2, 2 );
   if (Use["MLP"])           histNn      = new TH1F( "MVA_MLP",           "MVA_MLP",           nbin, -1.25, 1.5 );
   if (Use["MLPBFGS"])       histNnbfgs  = new TH1F( "MVA_MLPBFGS",       "MVA_MLPBFGS",       nbin, -1.25, 1.5 );
   if (Use["MLPBNN"])        histNnbnn   = new TH1F( "MVA_MLPBNN",        "MVA_MLPBNN",        nbin, -1.25, 1.5 );
   if (Use["CFMlpANN"])      histNnC     = new TH1F( "MVA_CFMlpANN",      "MVA_CFMlpANN",      nbin,  0, 1 );
   if (Use["TMlpANN"])       histNnT     = new TH1F( "MVA_TMlpANN",       "MVA_TMlpANN",       nbin, -1.3, 1.3 );
   if (Use["BDT"])           histBdt     = new TH1F( "MVA_BDT",           "MVA_BDT",           nbin, -0.8, 0.8 );
   if (Use["BDTD"])          histBdtD    = new TH1F( "MVA_BDTD",          "MVA_BDTD",          nbin, -0.8, 0.8 );
   if (Use["BDTG"])          histBdtG    = new TH1F( "MVA_BDTG",          "MVA_BDTG",          nbin, -1.0, 1.0 );
   if (Use["RuleFit"])       histRf      = new TH1F( "MVA_RuleFit",       "MVA_RuleFit",       nbin, -2.0, 2.0 );
   if (Use["SVM_Gauss"])     histSVMG    = new TH1F( "MVA_SVM_Gauss",     "MVA_SVM_Gauss",     nbin,  0.0, 1.0 );
   if (Use["SVM_Poly"])      histSVMP    = new TH1F( "MVA_SVM_Poly",      "MVA_SVM_Poly",      nbin,  0.0, 1.0 );
   if (Use["SVM_Lin"])       histSVML    = new TH1F( "MVA_SVM_Lin",       "MVA_SVM_Lin",       nbin,  0.0, 1.0 );
   if (Use["FDA_MT"])        histFDAMT   = new TH1F( "MVA_FDA_MT",        "MVA_FDA_MT",        nbin, -2.0, 3.0 );
   if (Use["FDA_GA"])        histFDAGA   = new TH1F( "MVA_FDA_GA",        "MVA_FDA_GA",        nbin, -2.0, 3.0 );
   if (Use["Category"])      histCat     = new TH1F( "MVA_Category",      "MVA_Category",      nbin, -2., 2. );
   if (Use["Plugin"])        histPBdt    = new TH1F( "MVA_PBDT",          "MVA_BDT",           nbin, -0.8, 0.8 );

   // PDEFoam also returns per-event error, fill in histogram, and also fill significance
   if (Use["PDEFoam"]) {
      histPDEFoam    = new TH1F( "MVA_PDEFoam",       "MVA_PDEFoam",              nbin,  0, 1 );
      histPDEFoamErr = new TH1F( "MVA_PDEFoamErr",    "MVA_PDEFoam error",        nbin,  0, 1 );
      histPDEFoamSig = new TH1F( "MVA_PDEFoamSig",    "MVA_PDEFoam significance", nbin,  0, 10 );
   }

   // Book example histogram for probability (the other methods are done similarly)
   TH1F *probHistFi(0), *rarityHistFi(0);
   if (Use["Fisher"]) {
      probHistFi   = new TH1F( "MVA_Fisher_Proba",  "MVA_Fisher_Proba",  nbin, 0, 1 );
      rarityHistFi = new TH1F( "MVA_Fisher_Rarity", "MVA_Fisher_Rarity", nbin, 0, 1 );
   }

   // Prepare input tree (this must be replaced by your data source)
   // in this example, there is a toy tree with signal and one with background events
   // we'll later on use only the "signal" events for the test in this example.
   //   
   TFile *input(0);
   TString dataFileName = "TightCuts_histos_Data.root";   
   TString MCFileName = "TightCuts_histos_officialMC.root";
   //MCFileName = "officialMC4tmva_tightCuts.root";

   // The two following numbers have to be synchronized with the ones used in the PROOF macro!
   Float_t B0_massFit = 5.2797; Float_t B0_sigmaFit = 0.012;

   Float_t B0_signal, innerSB, outerSB;
   B0_signal = 2.;
   //innerSB = 5. ; outerSB = 9.;
   //innerSB = 4. ; outerSB = 6.;
   innerSB = 6. ; outerSB = 9.;
   TString B0limits = TString::Format("%.1fSig_%.1fto%.1fSB",B0_signal,innerSB,outerSB);
   B0limits.ReplaceAll(".","p") ;

   Double_t mumu_mass = 0;
   Int_t n_psi = 0;
   if (psi_nS.EqualTo("psi2S")) {
     n_psi = 2;
     mumu_mass = psi2S_mass; 
     MCFileName = "officialMC4tmva_"+psi_nS+"_tightCuts_"+B0limits+".root";
   } else {
     n_psi = 1;
     mumu_mass = jpsi_mass;
     //MCFileName = "officialMC_noPtEtaCuts_"+psi_nS+"_tightCuts_"+B0limits+".root";
     //MCFileName = "officialMC_"+psi_nS+"_Bd2MuMuKPi_"+B0limits+".root";
     MCFileName = "officialMC_noPtEtaCuts_"+psi_nS+"_Bd2MuMuKPi_"+B0limits+".root";
   }
   //dataFileName = "Data_"+psi_nS+"_tightCuts_"+B0limits+".root";
   dataFileName = "Data_"+psi_nS+"_"+B0limits+".root";

   Double_t mumu_mass2 = mumu_mass*mumu_mass;

   TString fileName = dataFileName;
   //fileName = MCFileName;

   Bool_t MC = kFALSE; TString dataset = "data";
   if (fileName.Contains("MC")) {
     MC = kTRUE;
     dataset = "MC";
   }
   TString path = "../";
   TString fullFileName = path+fileName;
   if (!gSystem->AccessPathName( fullFileName )) 
      input = TFile::Open( fullFileName ); // check if file in local directory exists
   //else    
     //input = TFile::Open( "http://root.cern.ch/files/tmva_class_example.root" ); // if not: download from ROOT server
   
   if (!input) {
      std::cout << "ERROR: could not open data file" << std::endl;
      exit(1);
   }
   std::cout << "--- TMVAClassificationApp    : Using input file: " << input->GetName() << std::endl;

   Int_t mod = 1000;
   
   cout <<"\nGetting histograms..." <<endl;
   TString histName = "myMuMuKPiMassAltZoom" ;
   TH1F* myPsiPKPiMassAltZoom_h=0, *myPsiPKPiMassAltZoom_BDT_h=0, *myPsiPKPiMassAltZoom_BDT_matched_h=0, *myPsiPKPiMassAltZoom_BDT_1B0_h=0, *myPsiPKPiMassAltZoom_BDT_1B0_matched_h=0 ;
   if ( (myPsiPKPiMassAltZoom_h = (TH1F*)input->Get( histName )) ) {
     myPsiPKPiMassAltZoom_BDT_h = (TH1F*) myPsiPKPiMassAltZoom_h->Clone( TString::Format("%s_BDT",myPsiPKPiMassAltZoom_h->GetName()) );
     myPsiPKPiMassAltZoom_BDT_h->SetTitle( TString::Format("%s BDT",myPsiPKPiMassAltZoom_h->GetTitle()) );
     myPsiPKPiMassAltZoom_BDT_h->Reset("ICE");

     myPsiPKPiMassAltZoom_BDT_matched_h = (TH1F*) myPsiPKPiMassAltZoom_BDT_h->Clone( TString::Format("%s_matched",myPsiPKPiMassAltZoom_BDT_h->GetName()) );
     myPsiPKPiMassAltZoom_BDT_matched_h->SetTitle( TString::Format("%s matched",myPsiPKPiMassAltZoom_BDT_h->GetTitle()) );

     myPsiPKPiMassAltZoom_BDT_1B0_h = (TH1F*) myPsiPKPiMassAltZoom_BDT_h->Clone( TString::Format("%s_1B0",myPsiPKPiMassAltZoom_BDT_h->GetName()) );
     myPsiPKPiMassAltZoom_BDT_1B0_h->SetTitle( TString::Format("%s 1B0",myPsiPKPiMassAltZoom_BDT_h->GetTitle()) );

     myPsiPKPiMassAltZoom_BDT_1B0_matched_h = (TH1F*) myPsiPKPiMassAltZoom_BDT_1B0_h->Clone( TString::Format("%s_matched",myPsiPKPiMassAltZoom_BDT_1B0_h->GetName()) );
     myPsiPKPiMassAltZoom_BDT_1B0_matched_h->SetTitle( TString::Format("%s matched",myPsiPKPiMassAltZoom_BDT_1B0_matched_h->GetTitle()) );

   } else cout <<"WARNING! Histogram \""+histName+"\" not found in file \""+input->GetName()+"\"" <<endl;


   histName = "B0_multiplicity_AC" ;
   TH1I* nB0AC_h, *nB0AC_BDT_h=0;
   if ( (nB0AC_h = (TH1I*)input->Get( histName )) ) {
     nB0AC_BDT_h = (TH1I*) nB0AC_h->Clone( TString::Format("%s_BDT",nB0AC_h->GetName()) );
     nB0AC_BDT_h->SetTitle( TString::Format("%s BDT",nB0AC_h->GetTitle()) );
     //nB0AC_BDT_h->Reset("ICES"); // moved after each BDT cut starting loop
   } else cout <<"WARNING! Histogram \""+histName+"\" not found in file \""+input->GetName()+"\"" <<endl;

   // reflections study
   histName = "myKKMass" ;
   TH1F* myKKMass_h = (TH1F*)input->Get( histName );
   //if (myKKMass_h) myKKMass_h->Reset("ICES"); // moved after each BDT cut starting loop
   if (!myKKMass_h)
     myKKMass_h = new TH1F("myKKMass","KK mass;m(K^{+}K^{-}) [GeV]", 200, 0.5, 2.5);
   TH1F* myKKMass_BDT_h = (TH1F*) myKKMass_h->Clone( TString::Format("%s_BDT",myKKMass_h->GetName()) );


   // efficiency

   // define num and denom
   vector< pair< pair<TH2F*, TH2F*>, Float_t> > effHist;
   //            name           xN     yN
   vector< pair< TString, pair< Int_t, Int_t> > > nameBinsGen; vector <Float_t> absEffMax;
   if (MC) {
     if (input) {
       //Int_t KPiMass2_bins = 40, MuMuPiMass2_bins = 40;
       Int_t KPiMass2_bins = 20, MuMuPiMass2_bins = 20;
       //
       Float_t cos_limit = 1.02; Float_t cos_margin = 0.04;
       cos_limit = 1.05; cos_margin = 0.05; // 42
       cos_limit = 1.1; cos_margin = 0.1; // 22
       Int_t cos_bins = 2*cos_limit/cos_margin;
       //
       Float_t phi_limit = 3.2; Float_t phi_margin = 0.05;
       phi_limit = 3.3; phi_margin = 0.1; // 66
       phi_limit = 3.4; phi_margin = 0.2; // 34
       phi_limit = 3.45; phi_margin = 0.3; // 23
       Int_t phi_bins = 2*phi_limit/phi_margin; 
       // this vector must be synchronized with the fill
       nameBinsGen.push_back(make_pair("psi2SPi_vs_KPi_Dalitz_BT_gen",make_pair(KPiMass2_bins, MuMuPiMass2_bins))); absEffMax.push_back(0.007);
       nameBinsGen.push_back(make_pair("psi2SPi_vs_KPi_BT_gen",make_pair(KPiMass2_bins, MuMuPiMass2_bins))); absEffMax.push_back(0.007);
       nameBinsGen.push_back(make_pair("cos_psi2S_helicityAngle_vs_KPiMass_BT_gen",make_pair(KPiMass2_bins, cos_bins))); absEffMax.push_back(0.003); 
       nameBinsGen.push_back(make_pair("planesAngle_vs_cos_psi2S_helicityAngle_BT_gen",make_pair(cos_bins, phi_bins))); absEffMax.push_back(0.003);
       for (Int_t iEff=0; iEff<(Int_t)(nameBinsGen.size()); ++iEff) {
	 TH2F* histGen = 0, *histReco = 0;
	 if ( (histGen = (TH2F*) input->Get( nameBinsGen[iEff].first.Data() )) ) {
	   histGen->Reset();
	   if (!nameBinsGen[iEff].first.Contains("planesAngle"))
	     histGen->SetBins(nameBinsGen[iEff].second.first, histGen->GetXaxis()->GetXmin(), histGen->GetXaxis()->GetXmax(), nameBinsGen[iEff].second.second, histGen->GetYaxis()->GetXmin(), histGen->GetYaxis()->GetXmax() );
	   else
	     histGen->SetBins(nameBinsGen[iEff].second.first, -cos_limit, cos_limit, nameBinsGen[iEff].second.second, -phi_limit, phi_limit );
	   TString nameReco = histGen->GetName();
	   if (!nameBinsGen[iEff].first.EqualTo("psi2SPi_vs_KPi_BT_gen"))
	     nameReco.ReplaceAll("BT_gen","BDT");
	   else nameReco = "psi2SPi_vs_KPi_B0constr_1B0_BDT";
	   histReco = (TH2F*) histGen->Clone(nameReco.Data());

	   TString titleReco = histGen->GetTitle();
	   titleReco.ReplaceAll("GEN before trigger","after BDT cut");
	   titleReco.ReplaceAll("GEN","after BDT cut");
	   histReco->SetTitle(titleReco);
	 } else {
	   cout <<"WARNING! Histogram \"" <<nameBinsGen[iEff].first <<"\" not found in file \"" <<input->GetName() <<"\"" <<endl;
	 }
	 effHist.push_back( make_pair( make_pair(histGen, histReco), absEffMax[iEff] ) );
       }

       // fill gen histos
       TString TNtupleName = "AA_genVars";
       TNtupleD* genVars = (TNtupleD*) input->Get(TNtupleName);
       Double_t massKPi, massMuMuPi, cosMuMu, phi, B0beauty;
       if (genVars) {
	 genVars->SetBranchAddress("massKPi",&massKPi);
	 genVars->SetBranchAddress("massMuMuPi",&massMuMuPi);
	 genVars->SetBranchAddress("cosMuMu",&cosMuMu);
	 genVars->SetBranchAddress("phi",&phi);
	 genVars->SetBranchAddress("B0beauty",&B0beauty);
 
	 mod = genVars->GetEntries() / 20;
	 for (Int_t i=0; i<genVars->GetEntries(); i++) {
	   if (i % mod == 0) std::cout << "--- ... Reading event " << i << " of " << genVars->GetEntries() << std::endl;
	   genVars->GetEntry(i);
	   //
	   Double_t mass2KPi = massKPi*massKPi, mass2MuMuPi = massMuMuPi*massMuMuPi;
	   if (effHist.size() > 0) {
	     if (effHist[0].first.first) effHist[0].first.first->Fill(mass2KPi, mass2MuMuPi);
	     if (effHist.size() > 1) {
	       if (effHist[1].first.first) effHist[1].first.first->Fill(massKPi, massMuMuPi);
	       if (effHist.size() > 2) {
		 if (effHist[2].first.first) effHist[2].first.first->Fill(massKPi, cosMuMu);
		 if (effHist.size() > 3) {
		   if (effHist[3].first.first) effHist[3].first.first->Fill(cosMuMu, phi);
		 }
	       }
	     }
	   }
	 }
       } else cout <<"WARNING! TNtupleD \"" <<TNtupleName <<"\" not found in file \"" <<input->GetName() <<"\"" <<endl;
     } else cout <<"WARNING! File \"" <<fullFileName <<"\" not found! Efficiency will not be calculated." <<endl;
   }
   
   vector <TH2F*> null;

   // Signal Dalitz
   TString sigLabel = "1B0_signalWin";
   TH2F* sigDalitz = 0;

   // Sidebands
   cout <<"Defining bkg histos" <<endl;
   vector< pair< TString, vector<TH2F*> > > bkgHist;
   // the following initialization must be syncronixed with the filling
   bkgHist.push_back(make_pair("psi2SPi_vs_KPi_Dalitz",null)); bkgHist.push_back(make_pair("psi2SPi_vs_KPi_masses",null)); bkgHist.push_back(make_pair("cos_Kstar_helicityAngle_fromMasses_vs_KPiMass",null)); bkgHist.push_back(make_pair("cos_Kstar_helicityAngle_fromMasses_vs_KPiMassSq",null)); bkgHist.push_back(make_pair("cos_Kstar_helicityAngle_fromMasses_vs_psiPiMass",null)); bkgHist.push_back(make_pair("cos_Kstar_helicityAngle_fromMasses_vs_psiPiMassSq",null)); bkgHist.push_back(make_pair("planesAngle_vs_cos_psi2S_helicityAngle",null));
   TString sb_name[] = {"sbs","leftSb","rightSb"};
   for (UInt_t iTH=0; iTH<bkgHist.size(); ++iTH) {
     if (iTH==0) {
       TString nameSig = bkgHist[iTH].first+"_"+sigLabel;
       if ( (sigDalitz = (TH2F*) input->Get( nameSig.Data() )) )
	 sigDalitz->SetName(TString::Format("%s_BDT",sigDalitz->GetName()));
       else cout <<"WARNING! Histogram \"" <<nameSig <<"\" not found" <<endl;
     }

     for (Int_t iSb=0; iSb < 3; ++iSb) {
       TString nameTH = bkgHist[iTH].first+"_"+sb_name[iSb];
       TH1* TH_forSb = 0;
       if ( (TH_forSb = (TH1*) input->Get( nameTH.Data() )) ) {
	 //TH_forSb->Reset(); // moved after each bdt cut starting loop
	 TH_forSb->SetName(TString::Format("%s_BDT",TH_forSb->GetName()));
       } else cout <<"WARNING! Histogram \"" <<nameTH <<"\" not found" <<endl;
       bkgHist[iTH].second.push_back( (TH2F*)TH_forSb );
     }
   }

   // Reflections
   cout <<"Defining 2D histos" <<endl;
   vector< pair< TString, vector<TH2F*> > > reflHist;
   // the following initialization must be syncronixed with the filling
   reflHist.push_back(make_pair("PiPiMass_vs_PiKMass",null)); reflHist.push_back(make_pair("KKMass_vs_PiKMass",null)); reflHist.push_back(make_pair("PKMass_vs_PiKMass",null)); reflHist.push_back(make_pair("PKMass_vs_PiPiMass",null)); reflHist.push_back(make_pair("PKMass_vs_KKMass",null)); reflHist.push_back(make_pair("PiPMass_vs_PiKMass",null)); reflHist.push_back(make_pair("PiPMass_vs_PiPiMass",null)); reflHist.push_back(make_pair("PiPMass_vs_KKMass",null)); reflHist.push_back(make_pair("PsiPPKMass_vs_PsiPPiKMass",null));

   vector< pair<TString, pair< pair<Float_t,Float_t>, pair<Float_t,Float_t> > > > region;
   region.push_back( make_pair("aV", make_pair( make_pair(5.15,5.45), make_pair(0,0) )) );
   //region.push_back( make_pair("leftSb", make_pair( make_pair(-outerSB,-innerSB), make_pair(0,0) )) );
   //region.push_back( make_pair("fromB0peak", make_pair( make_pair(-B0_signal,+B0_signal), make_pair(0,0) )) );
   //region.push_back( make_pair("rightSb", make_pair( make_pair(+innerSB,+outerSB), make_pair(0,0) )) );

   UInt_t nRegions = region.size();

   for (UInt_t iTH=0; iTH<reflHist.size(); ++iTH) {
     for (UInt_t iReg=0; iReg < nRegions; ++iReg) {
       TString nameTH = "my"+reflHist[iTH].first+"_"+region[iReg].first;
       TH1* TH_forRefl = 0;
       if ( (TH_forRefl = (TH1*) input->Get( nameTH.Data() )) ) {
	 //TH_forRefl->Reset(); // moved after each BDT cut starting loop 
	 TH_forRefl->SetName(TString::Format("%s_BDT_1B0",TH_forRefl->GetName()));
       } else cout <<"WARNING! Histogram \"" <<nameTH <<"\" not found" <<endl;
       reflHist[iTH].second.push_back( (TH2F*)TH_forRefl );
     }
   }

   // --- Event loop

   // Prepare the event tree
   // - here the variable names have to corresponds to your tree
   // - you can use the same variables as above which is slightly faster,
   //   but of course you can use different ones and copy the values inside the event loop
   //
   //std::cout << "--- Select signal sample" << std::endl;
   cout <<"\nGetting tree..." <<endl;
   TTree* theTree =  (TTree*)input->Get("mva_variables_all");
   if (!theTree) {
     cout <<"No TTree named \"" <<"mva_variables_all\" found in file " <<"\"" <<input->GetName() <<"\"\nAborting." <<endl;
     abort();
   }
   /*
   Float_t userVar1, userVar2;
   theTree->SetBranchAddress( "var1", &userVar1 );
   theTree->SetBranchAddress( "var2", &userVar2 );
   theTree->SetBranchAddress( "var3", &var3 );
   theTree->SetBranchAddress( "var4", &var4 );
   */
   for (Int_t i=0; i<BDT_vars_number; ++i) {
     cout <<"SettingBranchAddress for branch \"" <<var_name[i] <<"\"" <<endl;
     if (theTree->GetBranch( var_name[i] ))
       theTree->SetBranchAddress( var_name[i], &(var[i]) );
     else
       cout <<"No branch named \"" <<var_name[i] <<"\" found in TTree \"" <<theTree->GetName() <<"\"" <<endl;
       //theTree->Print();
   }

   TH1F* BDT_var_h = new TH1F[BDT_vars_number];
   TH1F* BDT_var_AC_h = new TH1F[BDT_vars_number];
   for(Int_t i = 0; i < BDT_vars_number; ++i) {
     TString tempName = TString::Format("%s_fromB0peak",var_name[i].Data());
     TH1F* temp = (TH1F*)input->Get( tempName );
     if (!temp) cout <<"No TH1F named \"" <<tempName <<" found in file \"" <<input->GetName() <<"\"" <<endl;
     else {
       temp->Reset("ICES");
       BDT_var_h[i] = *temp ; BDT_var_AC_h[i] = *temp ;   
       BDT_var_h[i].SetName( var_name[i] );
       BDT_var_h[i].SetTitle( var_name[i] );
       BDT_var_AC_h[i].SetName( TString::Format("%s_BDT", var_name[i].Data()) );
       BDT_var_AC_h[i].SetTitle( TString::Format("%s BDT", var_name[i].Data()) );
     }
   }

   Float_t myB0MassAlt, nB0AC, matchedB0;
   theTree->SetBranchAddress("B0_mass",&myB0MassAlt);
   theTree->SetBranchAddress("B0_multiplicity",&nB0AC);
   if (fileName.Contains("MC"))
     theTree->SetBranchAddress("B0_matched",&matchedB0);
   else
     matchedB0 = 0;

   // reflections check
   Float_t myKKMass, myPsiKKMass;
   Float_t Bs_free, f0_free;
   Float_t myPiPiMass, myPKMass, myPiPMass;
   Float_t myJpsiPiKMass, myJpsiPKMass;
   theTree->SetBranchAddress("PiPi_mass",&myPiPiMass);
   theTree->SetBranchAddress("KK_mass",&myKKMass);
   theTree->SetBranchAddress("PsiKK_mass",&myPsiKKMass);
   theTree->SetBranchAddress("Bs_free",&Bs_free);
   theTree->SetBranchAddress("f0_free",&f0_free);
   theTree->SetBranchAddress("PK_mass",&myPKMass);
   theTree->SetBranchAddress("PiP_mass",&myPiPMass);
   theTree->SetBranchAddress("PsiPK_mass",&myJpsiPKMass);

   // AA and efficiency
   Float_t massKPi, massMuMuPi, cosMuMu, phi, B0beauty;
   theTree->SetBranchAddress("KPi_mass",&massKPi);
   theTree->SetBranchAddress("PsiPi_mass",&massMuMuPi);
   theTree->SetBranchAddress("cosTheta_psi",&cosMuMu);
   theTree->SetBranchAddress("phi",&phi);
   theTree->SetBranchAddress("B0beauty",&B0beauty);

   // Efficiency calculator for cut method
   Int_t    nSelCutsGA = 0;
   Double_t effS       = 0.7;

   std::vector<Float_t> vecVar(4); // vector for EvaluateMVA tests

   Int_t maxEvents = theTree->GetEntries();
   //maxEvents = 300000;

   std::cout << "--- The total number of events in the tree is " << theTree->GetEntries() << std::endl;
   std::cout << "--- Processing the first "  << maxEvents << " events" << std::endl;
   TStopwatch sw;
   sw.Start();
   

   vector <Float_t> bdt_cut;
   if (psi_nS.EqualTo("JPsi")) {
     //bdt_cut.push_back(0.0); bdt_cut.push_back(-0.05); bdt_cut.push_back(0.05); bdt_cut.push_back(0.1); bdt_cut.push_back(0.15); bdt_cut.push_back(0.2);
     bdt_cut.push_back(0.0); bdt_cut.push_back(0.05);
   } else if (psi_nS.EqualTo("psi2S")) {
     bdt_cut.push_back(0.); bdt_cut.push_back(0.05); bdt_cut.push_back(0.1);
   }

   for (UInt_t iCut=0; iCut<bdt_cut.size(); iCut++) {
     cout <<"\nStarting selection with bdt > " <<bdt_cut[iCut] <<endl;
     cout <<"\nRemember to ->Reset() any histogram before filling it again!\n" <<endl;

     TString outputName = TString::Format("BDTCutAt%.2f",bdt_cut[iCut]);
     outputName.ReplaceAll(".","p");
     TFile *target  = new TFile( "TMVApp_"+dataset+"_with"+outputName+"_"+psi_nS+"_"+B0limits+".root","RECREATE" );

     TNtupleD* AA_NTuple = new TNtupleD("AA_vars","AA variables","massKPi:massMuMuPi:cosMuMu:phi:B0beauty");

     if (Use["Likelihood"   ])   histLk     ->Reset();
     if (Use["LikelihoodD"  ])   histLkD    ->Reset();
     if (Use["LikelihoodPCA"])   histLkPCA  ->Reset();
     if (Use["LikelihoodKDE"])   histLkKDE  ->Reset();
     if (Use["LikelihoodMIX"])   histLkMIX  ->Reset();
     if (Use["PDERS"        ])   histPD     ->Reset();
     if (Use["PDERSD"       ])   histPDD    ->Reset();
     if (Use["PDERSPCA"     ])   histPDPCA  ->Reset();
     if (Use["KNN"          ])   histKNN    ->Reset();
     if (Use["HMatrix"      ])   histHm     ->Reset();
     if (Use["Fisher"       ])   histFi     ->Reset();
     if (Use["FisherG"      ])   histFiG    ->Reset();
     if (Use["BoostedFisher"])   histFiB    ->Reset();
     if (Use["LD"           ])   histLD     ->Reset();
     if (Use["MLP"          ])   histNn     ->Reset();
     if (Use["MLPBFGS"      ])   histNnbfgs ->Reset();
     if (Use["MLPBNN"       ])   histNnbnn  ->Reset();
     if (Use["CFMlpANN"     ])   histNnC    ->Reset();
     if (Use["TMlpANN"      ])   histNnT    ->Reset();
     if (Use["BDT"          ])   histBdt    ->Reset();
     if (Use["BDTD"         ])   histBdtD   ->Reset();
     if (Use["BDTG"         ])   histBdtG   ->Reset();
     if (Use["RuleFit"      ])   histRf     ->Reset();
     if (Use["SVM_Gauss"    ])   histSVMG   ->Reset();
     if (Use["SVM_Poly"     ])   histSVMP   ->Reset();
     if (Use["SVM_Lin"      ])   histSVML   ->Reset();
     if (Use["FDA_MT"       ])   histFDAMT  ->Reset();
     if (Use["FDA_GA"       ])   histFDAGA  ->Reset();
     if (Use["Category"     ])   histCat    ->Reset();
     if (Use["Plugin"       ])   histPBdt   ->Reset();
     if (Use["PDEFoam"]) {
       histPDEFoam   ->Reset();
       histPDEFoamErr->Reset();         
       histPDEFoamSig->Reset(); }         
     if (Use["Fisher"])   {
       probHistFi  ->Reset();
       rarityHistFi->Reset(); }

     AA_NTuple->Reset();

     for (Int_t i=0; i<BDT_vars_number; ++i)
       BDT_var_h[i].Reset();

     myKKMass_h->Reset();

     nB0AC_BDT_h->Reset("ICES");
     myPsiPKPiMassAltZoom_BDT_h->Reset();
     //
     for (Int_t i=0; i<BDT_vars_number; ++i)
       BDT_var_AC_h[i].Reset();
     //
     myKKMass_BDT_h->Reset();
     myPsiPKPiMassAltZoom_BDT_matched_h->Reset();
     myPsiPKPiMassAltZoom_BDT_1B0_h->Reset();
     
     sigDalitz->Reset();

     for (UInt_t iTH=0; iTH<bkgHist.size(); ++iTH)
       for (Int_t iSb=0; iSb < 3; ++iSb)
	 if (bkgHist[iTH].second[iSb]) bkgHist[iTH].second[iSb]->Reset();
     
     for (UInt_t iTH=0; iTH<reflHist.size(); ++iTH)
       for (UInt_t iReg=0; iReg < nRegions; ++iReg)
	 if (reflHist[iTH].second[iReg]) reflHist[iTH].second[iReg]->Reset();

     myPsiPKPiMassAltZoom_BDT_1B0_matched_h->Reset();

     if (MC)
       for (Int_t iEff=0; iEff<(Int_t)(effHist.size()); ++iEff)
	 if (effHist[iEff].first.second) effHist[iEff].first.second->Reset();

     mod = maxEvents / 20;
     for (Long64_t iEvt=0; iEvt<maxEvents; iEvt++) {

       if (iEvt % mod == 0) std::cout << "--- ... Processing event " << iEvt << " of " << maxEvents << std::endl;

       theTree->GetEntry(iEvt); // updating branches!

       Double_t AA_var[] = {massKPi, massMuMuPi, cosMuMu, phi, B0beauty};

       Float_t B0mass_gapFromFit = myB0MassAlt - B0_massFit;
       if (nRegions > 0) {
	 region[0].second.second = make_pair(myB0MassAlt,1) ;
	 for (UInt_t iReg=1; iReg<nRegions; ++iReg)
	   region[iReg].second.second = make_pair(B0mass_gapFromFit,B0_sigmaFit) ;
       }

       Float_t mass2KPi = massKPi*massKPi, mass2MuMuPi = massMuMuPi*massMuMuPi; 
       Float_t cosTheta_Kstar_fromMasses = cosTheta_FromMasses_host(mass2KPi, mass2MuMuPi, mumu_mass*mumu_mass, myB0MassAlt*myB0MassAlt, kaonCh_mass2, pionCh_mass2);

       // --- Return the MVA outputs and fill into histograms
       // this must be done after the TTree->GetEntry() of course!

       if (Use["CutsGA"]) {
         // Cuts is a special case: give the desired signal efficienciy
         Bool_t passed = reader->EvaluateMVA( "CutsGA method", effS );
         if (passed) nSelCutsGA++;
       }

       if (Use["Likelihood"   ])   histLk     ->Fill( reader->EvaluateMVA( "Likelihood method"    ) );
       if (Use["LikelihoodD"  ])   histLkD    ->Fill( reader->EvaluateMVA( "LikelihoodD method"   ) );
       if (Use["LikelihoodPCA"])   histLkPCA  ->Fill( reader->EvaluateMVA( "LikelihoodPCA method" ) );
       if (Use["LikelihoodKDE"])   histLkKDE  ->Fill( reader->EvaluateMVA( "LikelihoodKDE method" ) );
       if (Use["LikelihoodMIX"])   histLkMIX  ->Fill( reader->EvaluateMVA( "LikelihoodMIX method" ) );
       if (Use["PDERS"        ])   histPD     ->Fill( reader->EvaluateMVA( "PDERS method"         ) );
       if (Use["PDERSD"       ])   histPDD    ->Fill( reader->EvaluateMVA( "PDERSD method"        ) );
       if (Use["PDERSPCA"     ])   histPDPCA  ->Fill( reader->EvaluateMVA( "PDERSPCA method"      ) );
       if (Use["KNN"          ])   histKNN    ->Fill( reader->EvaluateMVA( "KNN method"           ) );
       if (Use["HMatrix"      ])   histHm     ->Fill( reader->EvaluateMVA( "HMatrix method"       ) );
       if (Use["Fisher"       ])   histFi     ->Fill( reader->EvaluateMVA( "Fisher method"        ) );
       if (Use["FisherG"      ])   histFiG    ->Fill( reader->EvaluateMVA( "FisherG method"       ) );
       if (Use["BoostedFisher"])   histFiB    ->Fill( reader->EvaluateMVA( "BoostedFisher method" ) );
       if (Use["LD"           ])   histLD     ->Fill( reader->EvaluateMVA( "LD method"            ) );
       if (Use["MLP"          ])   histNn     ->Fill( reader->EvaluateMVA( "MLP method"           ) );
       if (Use["MLPBFGS"      ])   histNnbfgs ->Fill( reader->EvaluateMVA( "MLPBFGS method"       ) );
       if (Use["MLPBNN"       ])   histNnbnn  ->Fill( reader->EvaluateMVA( "MLPBNN method"        ) );
       if (Use["CFMlpANN"     ])   histNnC    ->Fill( reader->EvaluateMVA( "CFMlpANN method"      ) );
       if (Use["TMlpANN"      ])   histNnT    ->Fill( reader->EvaluateMVA( "TMlpANN method"       ) );
       if (Use["BDT"          ])   histBdt    ->Fill( reader->EvaluateMVA( "BDT method"           ) );
       if (Use["BDTD"         ])   histBdtD   ->Fill( reader->EvaluateMVA( "BDTD method"          ) );
       if (Use["BDTG"         ])   histBdtG   ->Fill( reader->EvaluateMVA( "BDTG method"          ) );
       if (Use["RuleFit"      ])   histRf     ->Fill( reader->EvaluateMVA( "RuleFit method"       ) );
       if (Use["SVM_Gauss"    ])   histSVMG   ->Fill( reader->EvaluateMVA( "SVM_Gauss method"     ) );
       if (Use["SVM_Poly"     ])   histSVMP   ->Fill( reader->EvaluateMVA( "SVM_Poly method"      ) );
       if (Use["SVM_Lin"      ])   histSVML   ->Fill( reader->EvaluateMVA( "SVM_Lin method"       ) );
       if (Use["FDA_MT"       ])   histFDAMT  ->Fill( reader->EvaluateMVA( "FDA_MT method"        ) );
       if (Use["FDA_GA"       ])   histFDAGA  ->Fill( reader->EvaluateMVA( "FDA_GA method"        ) );
       if (Use["Category"     ])   histCat    ->Fill( reader->EvaluateMVA( "Category method"      ) );
       if (Use["Plugin"       ])   histPBdt   ->Fill( reader->EvaluateMVA( "P_BDT method"         ) );

       // Retrieve also per-event error
       if (Use["PDEFoam"]) {
         Double_t val = reader->EvaluateMVA( "PDEFoam method" );
         Double_t err = reader->GetMVAError();
         histPDEFoam   ->Fill( val );
         histPDEFoamErr->Fill( err );         
         if (err>1.e-50) histPDEFoamSig->Fill( val/err );
       }         

       // Retrieve probability instead of MVA output
       if (Use["Fisher"])   {
         probHistFi  ->Fill( reader->GetProba ( "Fisher method" ) );
         rarityHistFi->Fill( reader->GetRarity( "Fisher method" ) );
       }

       for (Int_t i=0; i<BDT_vars_number; ++i)
	 BDT_var_h[i].Fill( var[i] );

       myKKMass_h->Fill( myKKMass );

       Double_t bdt = reader->EvaluateMVA("BDT method");
       if (bdt > bdt_cut[iCut]) {

	 for (Int_t i=0; i<BDT_vars_number; ++i)
	   BDT_var_AC_h[i].Fill( var[i] );
	 //
	 myKKMass_BDT_h->Fill( myKKMass );


	 // removing Bs hereafter
	 if (Bs_free < 0.5  ||  f0_free < 0.5)
	   continue;

	 nB0AC_BDT_h->Fill( nB0AC );
	 myPsiPKPiMassAltZoom_BDT_h->Fill( myB0MassAlt );

	 if (matchedB0 > 0)
	   myPsiPKPiMassAltZoom_BDT_matched_h->Fill( myB0MassAlt );

	 if (nB0AC > 0.5  &&  nB0AC < 1.5) {
	   AA_NTuple->Fill( AA_var );
	   myPsiPKPiMassAltZoom_BDT_1B0_h->Fill( myB0MassAlt );

	   // signal Dalitz
	   Float_t sigWidth = 1.5;
	   sigWidth = 1.; B0_sigmaFit = 0.028; // from Johnson fit 
	   if ( fabs(B0mass_gapFromFit) < sigWidth*B0_sigmaFit ) 
	     sigDalitz->Fill(mass2KPi, mass2MuMuPi);

	   // Sidebands
	   innerSB = 4.5; outerSB = 6;
	   pair <Float_t,Float_t> sbLimits[] = {make_pair(-outerSB,-innerSB),make_pair(+innerSB,+outerSB)};
	   for (UInt_t iTH=0; iTH<bkgHist.size(); ++iTH) {
	     for (Int_t iSb=1; iSb < 3; ++iSb) {
	       if ( B0mass_gapFromFit > sbLimits[iSb-1].first*B0_sigmaFit  &&  B0mass_gapFromFit < sbLimits[iSb-1].second*B0_sigmaFit) {
		 switch ( iTH ) { // The following order has to match the initialization one
		 case 0:
		   if (Dalitz_contour_host(mass2KPi, mass2MuMuPi, kTRUE, n_psi)) {
		     if (bkgHist[iTH].second[0]) bkgHist[iTH].second[0]->Fill(mass2KPi, mass2MuMuPi);
		     if (bkgHist[iTH].second[iSb]) bkgHist[iTH].second[iSb]->Fill(mass2KPi, mass2MuMuPi);
		   }
		   break;
		 case 1:
		   if (Dalitz_contour_host(massKPi, massMuMuPi, kFALSE, n_psi)) {
		     if (bkgHist[iTH].second[0]) bkgHist[iTH].second[0]->Fill(massKPi, massMuMuPi);
		     if (bkgHist[iTH].second[iSb]) bkgHist[iTH].second[iSb]->Fill(massKPi, massMuMuPi);
		   }
		   break;
		 case 2:
		   if (bkgHist[iTH].second[0]) bkgHist[iTH].second[0]->Fill(massKPi, cosTheta_Kstar_fromMasses);
		   if (bkgHist[iTH].second[iSb]) bkgHist[iTH].second[iSb]->Fill(massKPi, cosTheta_Kstar_fromMasses);
		   break;
		 case 3:
		   if (bkgHist[iTH].second[0]) bkgHist[iTH].second[0]->Fill(mass2KPi, cosTheta_Kstar_fromMasses);
		   if (bkgHist[iTH].second[iSb]) bkgHist[iTH].second[iSb]->Fill(mass2KPi, cosTheta_Kstar_fromMasses);
		   break;
		 case 4:
		   if (bkgHist[iTH].second[0]) bkgHist[iTH].second[0]->Fill(massMuMuPi, cosTheta_Kstar_fromMasses);
		   if (bkgHist[iTH].second[iSb]) bkgHist[iTH].second[iSb]->Fill(massMuMuPi, cosTheta_Kstar_fromMasses);
		   break;
		 case 5:
		   if (bkgHist[iTH].second[0]) bkgHist[iTH].second[0]->Fill(mass2MuMuPi, cosTheta_Kstar_fromMasses);
		   if (bkgHist[iTH].second[iSb]) bkgHist[iTH].second[iSb]->Fill(mass2MuMuPi, cosTheta_Kstar_fromMasses);
		   break;
		 case 6:
		   if (bkgHist[iTH].second[0]) bkgHist[iTH].second[0]->Fill(cosMuMu, phi);
		   if (bkgHist[iTH].second[iSb]) bkgHist[iTH].second[iSb]->Fill(cosMuMu, phi);
		   break;
		 default:
		   cout <<"WARNING! No filling defined for " <<bkgHist[iTH].first <<endl;
		   break;
		 }
	       }
	     }
	   }

	   // Reflections
	   for (UInt_t iTH=0; iTH<reflHist.size(); ++iTH) {
	     for (UInt_t iReg=0; iReg < nRegions; ++iReg) {
	       if ( region[iReg].second.second.first > region[iReg].second.first.first*region[iReg].second.second.second
		    &&  region[iReg].second.second.first < region[iReg].second.first.second*region[iReg].second.second.second) {
		 switch ( iTH ) { // The following order has to match the initialization one
		 case 0:
		   if (reflHist[iTH].second[iReg]) reflHist[iTH].second[iReg]->Fill(myPiPiMass, massKPi);
		   break;
		 case 1:
		   if (reflHist[iTH].second[iReg]) reflHist[iTH].second[iReg]->Fill(myKKMass, massKPi);
		   break;
		 case 2:
		   if (reflHist[iTH].second[iReg]) reflHist[iTH].second[iReg]->Fill(myPKMass, massKPi);
		   break;
		 case 3:
		   if (reflHist[iTH].second[iReg]) reflHist[iTH].second[iReg]->Fill(myPKMass, myPiPiMass);
		   break;
		 case 4:
		   if (reflHist[iTH].second[iReg]) reflHist[iTH].second[iReg]->Fill(myPKMass, myKKMass);
		   break;
		 case 5:
		   if (reflHist[iTH].second[iReg]) reflHist[iTH].second[iReg]->Fill(myPiPMass, massKPi);
		   break;
		 case 6:
		   if (reflHist[iTH].second[iReg]) reflHist[iTH].second[iReg]->Fill(myPiPMass, myPiPiMass);
		   break;
		 case 7:
		   if (reflHist[iTH].second[iReg]) reflHist[iTH].second[iReg]->Fill(myPiPMass, myKKMass);
		   break;
		 case 8:
		   if (reflHist[iTH].second[iReg]) reflHist[iTH].second[iReg]->Fill(myB0MassAlt, myJpsiPKMass);
		   break;
		 default:
		   cout <<"WARNING! No filling defined for " <<reflHist[iTH].first <<endl;
		   break;
		 }
	       }
	     }
	   }

	   if (matchedB0 > 0) {
	     myPsiPKPiMassAltZoom_BDT_1B0_matched_h->Fill( myB0MassAlt );

	     if (MC) {
	       if ( fabs(B0mass_gapFromFit) < sigWidth*B0_sigmaFit ) {
		 if (effHist.size() > 0) {
		   if (effHist[0].first.second) effHist[0].first.second->Fill(mass2KPi, mass2MuMuPi);
		   if (effHist.size() > 1) {
		     if (effHist[1].first.second) effHist[1].first.second->Fill(massKPi, massMuMuPi);
		     if (effHist.size() > 2) {
		       if (effHist[2].first.second) effHist[2].first.second->Fill(massKPi, cosMuMu);
		       if (effHist.size() > 3) {
			 if (effHist[3].first.second) effHist[3].first.second->Fill(cosMuMu, phi);
		       }
		     }
		   }
		 }
	       }
	     }
	   } // if (matchedB0 > 0)

	 } // if (nB0AC > 0.5  &&  nB0AC < 1.5)

       } // if (bdt > bdt_cut[iCut])

     } // for (Long64_t iEvt=0; iEvt<theTree->GetEntries(); iEvt++)

     // Get elapsed time
     sw.Stop();
     std::cout << "--- End of event loop: "; sw.Print();

     // Get efficiency for cuts classifier
     if (Use["CutsGA"]) std::cout << "--- Efficiency for CutsGA method: " << double(nSelCutsGA)/theTree->GetEntries()
				  << " (for a required signal efficiency of " << effS << ")" << std::endl;

     if (Use["CutsGA"]) {

       // test: retrieve cuts for particular signal efficiency
       // CINT ignores dynamic_casts so we have to use a cuts-secific Reader function to acces the pointer  
       TMVA::MethodCuts* mcuts = reader->FindCutsMVA( "CutsGA method" ) ;

       if (mcuts) {      
         std::vector<Double_t> cutsMin;
         std::vector<Double_t> cutsMax;
         mcuts->GetCuts( 0.7, cutsMin, cutsMax );
         std::cout << "--- -------------------------------------------------------------" << std::endl;
         std::cout << "--- Retrieve cut values for signal efficiency of 0.7 from Reader" << std::endl;
         for (UInt_t ivar=0; ivar<cutsMin.size(); ivar++) {
	   std::cout << "... Cut: " 
		     << cutsMin[ivar] 
		     << " < \"" 
		     << mcuts->GetInputVar(ivar)
		     << "\" <= " 
		     << cutsMax[ivar] << std::endl;
         }
         std::cout << "--- -------------------------------------------------------------" << std::endl;
       }
     }

     // --- Write histograms
     if (Use["Likelihood"   ])   histLk     ->Write();
     if (Use["LikelihoodD"  ])   histLkD    ->Write();
     if (Use["LikelihoodPCA"])   histLkPCA  ->Write();
     if (Use["LikelihoodKDE"])   histLkKDE  ->Write();
     if (Use["LikelihoodMIX"])   histLkMIX  ->Write();
     if (Use["PDERS"        ])   histPD     ->Write();
     if (Use["PDERSD"       ])   histPDD    ->Write();
     if (Use["PDERSPCA"     ])   histPDPCA  ->Write();
     if (Use["KNN"          ])   histKNN    ->Write();
     if (Use["HMatrix"      ])   histHm     ->Write();
     if (Use["Fisher"       ])   histFi     ->Write();
     if (Use["FisherG"      ])   histFiG    ->Write();
     if (Use["BoostedFisher"])   histFiB    ->Write();
     if (Use["LD"           ])   histLD     ->Write();
     if (Use["MLP"          ])   histNn     ->Write();
     if (Use["MLPBFGS"      ])   histNnbfgs ->Write();
     if (Use["MLPBNN"       ])   histNnbnn  ->Write();
     if (Use["CFMlpANN"     ])   histNnC    ->Write();
     if (Use["TMlpANN"      ])   histNnT    ->Write();
     if (Use["BDT"          ])   histBdt    ->Write();
     if (Use["BDTD"         ])   histBdtD   ->Write();
     if (Use["BDTG"         ])   histBdtG   ->Write(); 
     if (Use["RuleFit"      ])   histRf     ->Write();
     if (Use["SVM_Gauss"    ])   histSVMG   ->Write();
     if (Use["SVM_Poly"     ])   histSVMP   ->Write();
     if (Use["SVM_Lin"      ])   histSVML   ->Write();
     if (Use["FDA_MT"       ])   histFDAMT  ->Write();
     if (Use["FDA_GA"       ])   histFDAGA  ->Write();
     if (Use["Category"     ])   histCat    ->Write();
     if (Use["Plugin"       ])   histPBdt   ->Write();

     // Write also error and significance histos
     if (Use["PDEFoam"]) { histPDEFoam->Write(); histPDEFoamErr->Write(); histPDEFoamSig->Write(); }

     // Write also probability hists
     if (Use["Fisher"]) { if (probHistFi != 0) probHistFi->Write(); if (rarityHistFi != 0) rarityHistFi->Write(); }

     TString plotsDir = psi_nS+"/"+B0limits+"/vars_"+outputName;
     if (fileName.Contains("MC"))
       plotsDir.Prepend("plots/mc/");
     else
       plotsDir.Prepend("plots/data/");

     gSystem->mkdir(plotsDir, true);

     AA_NTuple->Write();

     for (Int_t i=0; i<BDT_vars_number; ++i) {
       BDT_var_h[i].Write(); BDT_var_AC_h[i].Write();
       BDT_var_AC_h[i].Scale( BDT_var_h[i].GetEntries() / BDT_var_AC_h[i].GetEntries() ); 
       BDT_var_AC_h[i].SetLineColor(kRed);
       //
       if (BDT_var_h[i].GetMaximum() > BDT_var_AC_h[i].GetMaximum()) {
	 BDT_var_h[i].Draw(); BDT_var_AC_h[i].Draw("same"); }
       else {
	 BDT_var_AC_h[i].Draw(); BDT_var_h[i].Draw("same"); }
       if (var_name[i].Contains("pointingAngle"))
	 gPad->SetLogy();
       //gPad->SaveAs( TString::Format("plots/vars_BDT/%s.png", BDT_var_h[i].GetName()) );
       gPad->SaveAs( TString::Format("%s/%s.png", plotsDir.Data(), BDT_var_h[i].GetName()) );
     }
     //
     myKKMass_h->Write(); myKKMass_BDT_h->Write();
     myKKMass_BDT_h->SetLineColor(kRed);
     // Reflections
     for (UInt_t iTH=0; iTH<reflHist.size(); ++iTH)
       for (UInt_t iReg=0; iReg < nRegions; ++iReg)
	 if (reflHist[iTH].second[iReg]) reflHist[iTH].second[iReg]->Write();

     sigDalitz->Write();

     // Sidebands
     for (UInt_t iTH=0; iTH<bkgHist.size(); ++iTH)
       for (Int_t iSb=0; iSb < 3; ++iSb)
	 if (bkgHist[iTH].second[iSb]) bkgHist[iTH].second[iSb]->Write();

     myKKMass_h->Draw(); myKKMass_BDT_h->Draw("same");
     gPad->SaveAs( TString::Format("%s/%s.png", plotsDir.Data(), myKKMass_h->GetName()) );
     //
     myPsiPKPiMassAltZoom_h->Write();
     nB0AC_h->Write();
     myPsiPKPiMassAltZoom_BDT_h->Write(); myPsiPKPiMassAltZoom_BDT_matched_h->Write();
     nB0AC_BDT_h->Write();
     myPsiPKPiMassAltZoom_BDT_1B0_h->Write(); myPsiPKPiMassAltZoom_BDT_1B0_matched_h->Write();

     // Relative and absolute efficiency

     // Dalitz countour
     TGraph *cont_up = 0; TGraph *cont_down = 0;
     Int_t m = 19800;
     Double_t x[19800], m23_max[19800], m23_min[19800];
     Double_t E2[19800], E3[19800];
	    
     Double_t m_mother = B0_mass; 
     Double_t m_dau1 = kaonCh_mass, m_dau2 = pionCh_mass, m_dau3 = mumu_mass ; 
	    
     Double_t m12_min = (m_dau1+m_dau2)*(m_dau1+m_dau2);
     Double_t m12_max = (m_mother-m_dau3)*(m_mother-m_dau3);
     Double_t step = (m12_max - m12_min)/(m-1);
	    
     x[0] = m12_min + 0.0001;
	    
     for (Int_t k=1; k<m; k++ ) 
       x[k] = x[k-1] + step;
	    
     Int_t n = 19799;
     for (Int_t i=0; i<n; i++) {
       E2[i] = (x[i] - m_dau1*m_dau1 + m_dau2*m_dau2)/(2*sqrt(x[i]));
       E3[i] = (m_mother*m_mother - x[i] - m_dau3*m_dau3)/(2*sqrt(x[i]));
       m23_min[i] = (E2[i]+E3[i])*(E2[i]+E3[i]) - TMath::Power((sqrt(E2[i]*E2[i] - m_dau2*m_dau2) + sqrt(E3[i]*E3[i] - m_dau3*m_dau3)),2);
       m23_max[i] = (E2[i]+E3[i])*(E2[i]+E3[i]) - TMath::Power((sqrt(E2[i]*E2[i] - m_dau2*m_dau2) - sqrt(E3[i]*E3[i] - m_dau3*m_dau3)),2);
     }

     cont_up = new TGraph(n,x,m23_min); cont_up->SetLineWidth(3); cont_up->SetLineColor(kBlack);
     cont_down = new TGraph(n,x,m23_max); cont_down->SetLineWidth(3); cont_down->SetLineColor(kBlack);


     TCanvas* dalitzC = new TCanvas("Dalitz_C","Dalitz C", 800, 600);
     dalitzC->SetRightMargin(0.2);
     for (Int_t iEff=0; iEff<(Int_t)(effHist.size()); ++iEff) {
       //cout <<"effHist[iEff].first.first = " <<effHist[iEff].first.first <<endl;
       //cout <<"effHist[iEff].first.second = " <<effHist[iEff].first.second <<endl;
       if (!effHist[iEff].first.first || !effHist[iEff].first.second) {
	 cout <<"WARNING! Skipping efficiency computation number " <<iEff <<"!" <<endl;
	 continue;
       }

       effHist[iEff].first.first->Write();
       effHist[iEff].first.second->Write();

       gStyle->SetOptStat("e") ;	 

       effHist[iEff].first.second->Draw("colz");
       Bool_t contour = kFALSE; contour = kTRUE;
       if (contour) { // Dalitz contour
	 cont_up->Draw("lsame"); cont_down->Draw("lsame"); 
       }  
       dalitzC->SaveAs( TString::Format("%s/%s.png", plotsDir.Data(), effHist[iEff].first.second->GetName()) );

       effHist[iEff].first.first->Draw("colz") ;

       if ( nameBinsGen[iEff].first.Contains("ngle") )
	 contour = kFALSE;
       if ( contour ) { // Dalitz contour
	 cont_up->Draw("lsame"); cont_down->Draw("lsame"); 
       }
       dalitzC->SaveAs( TString::Format("%s/%s.png", plotsDir.Data(), effHist[iEff].first.first->GetName()) );
	  
       TString effName = effHist[iEff].first.second->GetName(); effName.ReplaceAll("BDT",outputName);
       cout <<"\nCreating efficiency TH2 for " <<effName <<endl ;
       Bool_t DalitzCheck = effName.Contains("psi2SPi_vs_KPi_",TString::kIgnoreCase) ? kTRUE : kFALSE;
       Bool_t massSquared = effName.Contains("Dalitz",TString::kIgnoreCase) ? kTRUE : kFALSE;
       Bool_t squaredDalitz = effName.Contains("vs_KPiMass",TString::kIgnoreCase) ? kTRUE : kFALSE;
       Bool_t squaredDalitz_2 = effName.Contains("vs_KPiMassSq",TString::kIgnoreCase) ? kTRUE : kFALSE;

       TH2D *hAbsEff, *hAbsEffErr, *hRelEff, *hRelEffInv, *hRelEffErr;
       TH1D *hAbsEffDistr, *hGenDistr, *hRecoDistr;
	    
       Float_t xLow = effHist[iEff].first.first->GetXaxis()->GetXmin() ;
       Float_t xHigh = effHist[iEff].first.first->GetXaxis()->GetXmax() ;
       const TArrayD* binXEdges = effHist[iEff].first.first->GetXaxis()->GetXbins(); // need to add safety check to use this
       Int_t nX = effHist[iEff].first.first->GetNbinsX() ;
       Float_t yLow = effHist[iEff].first.first->GetYaxis()->GetXmin() ;
       Float_t yHigh = effHist[iEff].first.first->GetYaxis()->GetXmax() ;
       const TArrayD* binYEdges = effHist[iEff].first.first->GetYaxis()->GetXbins(); // need to add safety check to use this
       Int_t nY = effHist[iEff].first.first->GetNbinsY() ;
       hAbsEff = new TH2D ( TString::Format("AbsEff_%s",effName.Data()), "AbsEff", nX,xLow,xHigh, nY,yLow,yHigh);
       hAbsEff->SetXTitle( effHist[iEff].first.first->GetXaxis()->GetTitle() ); hAbsEff->SetYTitle( effHist[iEff].first.first->GetYaxis()->GetTitle() );
       hRelEff = new TH2D ( TString::Format("RelEff_%s",effName.Data()), "Relative efficiency", nX,xLow,xHigh, nY,yLow,yHigh);
       hRelEffInv = new TH2D( TString::Format("RelEffInv_%s",effName.Data()), "Inverse relative efficiency", nX,xLow,xHigh, nY,yLow,yHigh);
       hRelEffErr = new TH2D ( TString::Format("RelEffErr_%s",effName.Data()), "Relative efficiency uncertainty", nX, xLow,xHigh, nY, yLow,yHigh);

       hRelEff->SetXTitle( effHist[iEff].first.first->GetXaxis()->GetTitle() ); hRelEff->SetYTitle( effHist[iEff].first.first->GetYaxis()->GetTitle() );
       hRelEffInv->SetXTitle( effHist[iEff].first.first->GetXaxis()->GetTitle() ); hRelEffInv->SetYTitle( effHist[iEff].first.first->GetYaxis()->GetTitle() );
       hAbsEffErr = new TH2D ( TString::Format("AbsEffErr_%s",effName.Data()), "AbsEffErr", nX,xLow,xHigh, nY,yLow,yHigh);
       hAbsEffErr->SetXTitle( effHist[iEff].first.first->GetXaxis()->GetTitle() ); hAbsEffErr->SetYTitle( effHist[iEff].first.first->GetYaxis()->GetTitle() );
       hAbsEffDistr = new TH1D ( TString::Format("hAbsEffDistr_%s",effName.Data()),"Efficiency distribution", 100, -0.001, 0.006);
       hGenDistr = new TH1D ( TString::Format("hGenDistr_%s",effName.Data()),"Bins' Gen entries distribution", 100, 0., 25000);
       hRecoDistr = new TH1D ( TString::Format("hRecoDistr_%s",effName.Data()),"Bins' Reco entries distribution", 110, 0., 55.);
       
       Int_t MyPalette[100];
       Double_t red[]   = { 1.0, 0.0, 1.0, 1.0 };
       Double_t green[] = { 1.0, 0.0, 0.0, 1.0 };
       Double_t blue[]  = { 1.0, 1.0, 0.0, 0.0 };
       Double_t stop[] = { 0.0, 0.5, 0.9, 1.0 };
       Int_t FI = TColor::CreateGradientColorTable(4, stop, red, green, blue, 100);
       for (int i=0; i<100; i++)
	 MyPalette[i] = FI+i;
       
       gStyle->SetPalette(100, MyPalette);
       
       Int_t genValue, recoValue;
       Double_t absEffValue, absEffErrValue;
       Double_t relEffValue, relEffErrValue;
       //
       Int_t count=0;
       Double_t sumAbsEff = 0., sumAbsEffErr = 0.;
       //
       for (Int_t i=0; i<=nX+1; i++) {
	 for (Int_t j=0; j<=nY+1; j++) {
	   absEffValue = 0.; absEffErrValue = 0.; // default values, was -0.0005
	   relEffValue = 0.; relEffErrValue = 0.; // important for a proper relEff computation

	   genValue = effHist[iEff].first.first->GetBinContent(i,j);
	   recoValue = effHist[iEff].first.second->GetBinContent(i,j);
	   //cout << "riga j=" << j << " e colonna i=" << i << " -> no. candidati generati = " << genBinCont[j][i] << " & no. ricostruiti = " << recoBinCont[j][i] << endl;
	   hGenDistr->Fill(genValue);
	   hRecoDistr->Fill(recoValue);
	   //
	   //if ( genValue > 500. && recoValue <= genValue ) // fix 500 with sth not hardcoded
	   if ( genValue > 0  &&  recoValue <= genValue )
	     //if ( recoValue > 2 )
	     {
	       absEffValue = (Double_t)recoValue / genValue;
	       absEffErrValue = (1. / genValue ) * (sqrt( genValue * absEffValue * (1 + absEffValue) ) );
	       //
	       // calculate weighted mean: two possible intervals: ]0,0.00225[ or ]0,0.005[
	       //if (absEffValue > 0. && absEffValue < 0.01) // 0.005  &&  0.01
	       if (absEffValue > 0.  &&  absEffErrValue > 0.) // 0.005  &&  -
		 {
		   //if (absEffValue < 0.0035) // 0.005 // tuned on Dalitz
		   Float_t mKP = hRelEff->GetXaxis()->GetBinCenter(i);
		   Float_t mPsiP = hRelEff->GetYaxis()->GetBinCenter(j);
		   if ( (!DalitzCheck  &&  !squaredDalitz  &&  !squaredDalitz_2) // list all the checks performed below
			|| (DalitzCheck  &&  Dalitz_contour_host(mKP,mPsiP,massSquared,n_psi)) 
			|| (squaredDalitz  &&  !squaredDalitz_2  &&  mKP > kaonCh_mass+pionCh_mass  &&  mKP < B0_mass-mumu_mass)
			|| (squaredDalitz_2  &&  mKP > TMath::Power(kaonCh_mass+pionCh_mass,2)  &&  mKP < TMath::Power(B0_mass-mumu_mass,2))
			)
		     {
		       relEffValue = absEffValue; relEffErrValue = absEffErrValue;
		       sumAbsEff += (absEffValue/(absEffErrValue * absEffErrValue)) ;
		       sumAbsEffErr += (1./(absEffErrValue * absEffErrValue));
		       count++ ;
		     }
		 }
	     }
	   hAbsEffDistr->Fill(absEffValue);
	   //cout << "raw-j=" << j << " e col-i=" << i << " -> no. gen-cand = " << genBinCont[j][i] << " & no. reco-cand = " << recoBinCont[j][i] << " & eff= " << absEff[j][i] << endl;
	   hAbsEff->SetBinContent(i,j, absEffValue); hAbsEff->SetBinError(i,j, absEffErrValue);
	   hAbsEffErr->SetBinContent(i,j, absEffErrValue);
	   hRelEff->SetBinContent(i,j, relEffValue); hRelEff->SetBinError(i,j, relEffErrValue);
	   hRelEffErr->SetBinContent(i,j, relEffErrValue);
	   if (relEffValue) hRelEffInv->SetBinContent(i,j, 1/relEffValue);
	   else hRelEffInv->SetBinContent(i,j, 0);
	 } // for (Int_t j=0; j<=nY+1; j++)
       } // for (Int_t i=0; i<=nX+1; i++)
       //
       Double_t weightedMeanEff = sumAbsEff/sumAbsEffErr;
       // Double_t weightedMeanEffErr = 1./sqrt(sumAbsEffErr);
       cout <<"count = " <<count << " with weighted mean = " << weightedMeanEff << " and weighted mean uncertainty = " << 1./sqrt(sumAbsEffErr) << endl; // about 10-3 with error 10-5

       //////////////////////////////////////// RELATIVE EFFICIENCY CALCULATION
       hRelEff->Scale(1./weightedMeanEff); hRelEffErr->Scale(1./weightedMeanEff); 
       hRelEffInv->Scale(weightedMeanEff);
       //
       hGenDistr->Write() ; hRecoDistr->Write() ;
       //
       hAbsEff->SetMaximum( (hAbsEff->GetSumOfWeights()/count)*3 );
       hAbsEff->Draw("colz"); hAbsEff->SetStats(kFALSE);
       if (contour) { // Dalitz contour
	 cont_up->Draw("lsame"); cont_down->Draw("lsame"); }
       dalitzC->SaveAs( TString::Format("%s/%s_absEff.png", plotsDir.Data(), effName.Data()) );
       hAbsEff->Write() ;

       hAbsEffErr->SetMaximum( (hAbsEffErr->GetSumOfWeights()/count)*3 );
       hAbsEffErr->Draw("colz"); hAbsEffErr->SetStats(kFALSE);
       if (contour) { // Dalitz contour
	 cont_up->Draw("lsame"); cont_down->Draw("lsame"); }
       dalitzC->SaveAs( TString::Format("%s/%s_absEffErr.png", plotsDir.Data(), effName.Data()) );
       hAbsEffErr->Write() ;

       hRelEff->SetMaximum( (hRelEff->GetSumOfWeights()/count)*2 );
       hRelEff->Draw("colz"); hRelEff->SetStats(kFALSE);
       hRelEff->SetMinimum(0.); //hRelEff->SetMaximum(2.2);
       if (contour) { // Dalitz contour
	 cont_up->Draw("lsame"); cont_down->Draw("lsame"); } 
       dalitzC->SaveAs( TString::Format("%s/%s_relEff.png", plotsDir.Data(), effName.Data()) );
       hRelEff->Write();

       hRelEffInv->Write();

       hRelEffErr->SetMaximum( (hRelEffErr->GetSumOfWeights()/count)*3 );
       hRelEffErr->Draw("colz"); hRelEffErr->SetStats(kFALSE);
       hRelEffErr->SetMinimum(0.); //hRelEffErr->SetMaximum(2.2);
       if (contour) { // Dalitz contour
	 cont_up->Draw("lsame"); cont_down->Draw("lsame"); } 
       dalitzC->SaveAs( TString::Format("%s/%s_relEffErr.png", plotsDir.Data(), effName.Data()) );
       hRelEffErr->Write();

       hAbsEffDistr->Draw() ; hAbsEffDistr->Write() ;

     /*
       //////////////////////////////////////// RELATIVE EFFICIENCY CALCULATION
       //   
       Double_t relEff[nX][nY];
       //Double_t relEffErr[nX][nY];
       //
       Double_t weightedMeanEff = sumAbsEff/sumAbsEffErr;
       // Double_t weightedMeanEffErr = 1./sqrt(sumAbsEffErr);
       //
       for (Int_t i=0; i<nX; i++) {
	 for (Int_t j=0; j<nY; j++) {
	   //genBinCont[i][j] = effHist[iEff].first.first->GetBinContent(i,j);
	   //recoBinCont[i][j] = dalitzReco->GetBinContent(i,j);	   
	   //if ( genBinCont[i][j] > 500. && recoBinCont[i][j] <= genBinCont[i][j] ) 
	   if ( recoBinCont[i][j] <= genBinCont[i][j] ) {
	     //absEff[i][j] = recoBinCont[i][j] / genBinCont[i][j];
	     //if (absEff[i][j] > 0. && absEff[i][j] < 0.05) // 0. && 0.005
	     relEff[i][j] = absEff[i][j]/weightedMeanEff;
	   }
	   else
	     relEff[i][j] = 0. ;
	   hRelEff->SetBinContent(i,j,relEff[i][j]);
	 }
       }
       //
       hRelEff->Draw("colz"); hRelEff->SetStats(kFALSE);
       hRelEff->SetMinimum(0.); hRelEff->SetMaximum(2.2);
       if (contour) { // Dalitz contour
	 cont_up->Draw("lsame"); cont_down->Draw("lsame");
       } 
       dalitzC->SaveAs( TString::Format("%s/%s_relEff.png", plotsDir.Data(), effName.Data()) );
       hRelEff->Write();

       hAbsEffDistr->Draw() ;
     */
     } // for (Int_t iEff=0; iEff<(Int_t)(effHist.size()); ++iEff)

     target->Close();

     std::cout << "--- Created root file: \"" <<target->GetName() <<"\" containing the MVA output histograms" << std::endl;
   } // for (UInt_t iCut=0; iCut<bdt_cut.size(); iCut++) 

   delete reader;
    
   std::cout << "==> TMVAClassificationApplication is done!\n" << endl ;
} 
