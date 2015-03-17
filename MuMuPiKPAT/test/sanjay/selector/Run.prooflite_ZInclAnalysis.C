{{
    //    gSystem->Exec("rm MySelector_MCAnalysis_*");
    /////////////////////////////////////////////////////
    //
    // INPUT DATA SAMPLE
    TDSet* dataset = new TDSet("TTree", "Z_data");
    //TDSet* dataset = new TDSet("TTree", "Z_data", "/mkcands");
    //
    // This version is with Leonardo's code (May 2014)
    //    //
    /////////////////////////////////
    // later version by Kai (30 of october)
    //
    // 2011
    //
    // 2012
    Bool_t MC = kFALSE; //MC = kTRUE;
    if ( MC ) {
      dataset->SetDirectory("/mkcands");
      dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/sanjay/MuOniaRun2012D_13Jul_MuMuPiPiPAT_ntpl_all.root");
    } else {
      Bool_t dsA = kFALSE, dsB = kFALSE, dsC = kFALSE, dsD = kFALSE ;
      //dsA = kTRUE ;
      dsB = kTRUE ;
      //dsC = kTRUE ;
      //dsD = kTRUE ;
      //
      // dataset A (288886 events)
      if (dsA) {
      /*    
	    dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012A.root");
      */
      // on /lustre
      dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012A_13Jul_JPsiPiPiPAT-full_datasetA.root");    
      // on /cmshome
      //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012A_13Jul_JPsiPiPiPAT-full_datasetA.root");
      }
      //
      // dataset B
      if (dsB) {
	/*
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012B_Job1.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012B_Job2.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012B_Job3.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012B_Job4.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012B_Job5.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012B_Job6.root");
	*/
	// on /lustre
	dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012B_13Jul_JPsiPiPiPAT-datasetB.root");    
	//
	//dataset->Add("/lustre/cms/store/user/lecriste/full_datasetB/MuOniaRun2012B_13Jul_JPsiPiPiPAT_ntpl_1-499.root");
	//dataset->Add("/lustre/cms/store/user/lecriste/full_datasetB_2/MuOniaRun2012B_13Jul_JPsiPiPiPAT-datasetB_2.root");
	// on /cmshome
	//dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012B_13Jul_JPsiPiPiPAT-datasetB.root");
      }
      //
      // dataset C
      if (dsC) {
	/*
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012C_Job1.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012C_Job2.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012C_Job3.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012C_Job4.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012C_Job5.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012C_Job6.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012C_Job7.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012C_Job8.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012C_Job9.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012C_Job10.root");
	*/
	// on /lustre
	dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012C_13Jul_JPsiPiPiPAT-full_datasetC_no3074.root");
	dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012C_13Jul_JPsiPiPiPAT_ntpl_3074.root");
	// on /cmshome
	//dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012C_13Jul_JPsiPiPiPAT-full_datasetC_no3074.root");
	//dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012C_13Jul_JPsiPiPiPAT_ntpl_3074.root");
      }
      //
      // dataset D
      if (dsD) {
	/*
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012D_Job1.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012D_Job2.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012D_Job3.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012D_Job4.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012D_Job5.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012D_Job6.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012D_Job7.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012D_Job8.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012D_Job9.root");
	  dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012D_Job10.root");
	*/
	// on /lustre
	dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012D_13Jul_JPsiPiPiPAT-datasetD_no856and1118and2110and3411.root");
	dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012D_13Jul_JPsiPiPiPAT-datasetD_856and1118and2110and3411.root");
	// on /cmshome
	//dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012D_13Jul_JPsiPiPiPAT-datasetD_856and1118and2110and3411.root");
	//dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012D_13Jul_JPsiPiPiPAT-datasetD_no856and1118and2110and3411.root");
      }
      /////////////////////////////////////////////////////////////////////////
      // test
      //dataset->Add("/cmshome/pompili/for-nairit-new/merged_MuOniaRun2012C_13Jul_JPsiPiPiPAT-datasetA_just9_random_files.root");
      /////////////////////////////////////////////////////////////////////////
    }
    //
    //
    gEnv->SetValue("Proof.StatsHist",1);
    gEnv->SetValue("Proof.StatsTrace",1);
    gEnv->SetValue("Proof.SlaveStatsTrace",1);
    //
    TString selector = "psiPrimePiK_MC";
    //
    TProof *p = TProof::Open("workers=8");
    
    // Processing...
    cout << ">> Processing " << selector << " ..." << endl;
    TString selectorcplus = selector;
    selectorcplus+= ".C+";
    //selectorcplus+= ".C"; // not working
    if ( MC ) 
      p->Process(dataset, selectorcplus, "MC");  
    else
      p->Process(dataset, selectorcplus);  
    
    //  gSystem->Exec("/opt/exp_soft/cms/slc5_amd64_gcc434/lcg/root/5.27.06b-cms16/test/ProofBench/SavePerfInfo.C("")");


//////////////////////////////
// how configure PROOF:
//
// root -l 
// root [1] TFile *f=TFile::Open("reduced2012D_Job6.root")
// root [2] TTree *tree=(TTree*)f->Get("X_data")
// root [3] tree->MakeSelector("PsiPrimePi_IOWA")
// Info in <TTreePlayer::MakeClass>: Files: PsiPrimePi_IOWA.h and PsiPrimePi_IOWA.C generated from TTree: X_data 
// (Int_t)0
// root [4] .q
//
// root [0] .x Run.prooflite_ZInclAnalysis.C
////////////////////////////

  }}

//rsync -vut --existing Run.prooflite_ZInclAnalysis.C cristella@cmssusy.ba.infn.it:/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/sanjay/selector/Run.prooflite_ZInclAnalysis.C
