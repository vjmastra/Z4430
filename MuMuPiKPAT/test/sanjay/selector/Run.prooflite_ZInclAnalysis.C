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
    Bool_t dsA = kFALSE, dsB = kFALSE, dsC = kFALSE, dsD = kFALSE ;
    dsA = kTRUE ;
    dsB = kTRUE ;
    dsC = kTRUE ;
    dsD = kTRUE ;
    Bool_t official = kFALSE; official = kTRUE;
    Bool_t noPtEtaCuts = kFALSE; noPtEtaCuts = kTRUE; 
    Bool_t genericMC = kFALSE; //genericMC = kTRUE;

    TString psi_nS = "psi2S"; psi_nS = "JPsi";
    TString B0massC = ""; //B0massC = "B0massC";
    
    if ( MC ) {
      dataset->SetDirectory("/mkcands");
      if ( !official ) {
	if ( psi_nS.EqualTo("psi2S") ) {
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/sanjay/MuOniaRun2012D_13Jul_MuMuPiPiPAT_ntpl_all.root");
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/sanjay/MuOniaRun2012D_13Jul_MuMuPiPiPAT_ntpl.root");
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/sanjay/MC_bkg/BsToPsiMuMu_23Mar_MuMuPiKPAT_ntpl.root");
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/BdToPsiKpi_18Mar_MuMuPiKPAT_ntpl_100.root");
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/BdToPsiKpi_18Mar_MuMuPiKPAT_ntpl_1K.root");
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/BdToPsiPHSPKpi_18Mar_MuMuPiKPAT_ntpl_500k.root");
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/BdToPsiPHSPKpi_18Mar_MuMuPiKPAT_ntpl_1K.root");
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/BdToPsiVLLKpi_18Mar_MuMuPiKPAT_ntpl_1K.root");
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/BdToPsiVLLKpi_18Mar_MuMuPiKPAT_ntpl_500k.root");
	} else if ( psi_nS.EqualTo("JPsi") ) {
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/BuSVSToJpsiK_18Mar_MuMuPiKPAT_ntpl.root");
	  // K*
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/BdToJpsiKstar_1_892_18Mar_MuMuPiKPAT_ntpl_1M.root");
	  dataset->Add("/lustrehome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/BdToJpsiKstar_1_892_18Mar_MuMuPiKPAT_ntpl_1M.root");
	  //dataset->Add("/lustrehome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/BdToJpsiKstar_2_1430_18Mar_MuMuPiKPAT_ntpl.root");
	  // pentaQuark
	  //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/BdToJpsiPPbar_18Mar_MuMuPPbarPAT_ntpl_10k.root");
	  // B0 mass constraint
	  //dataset->Add("/lustre/cms/store/user/nsur/BdToJpsiKPi_BFilter_MSEL5_TuneZ2star_8TeV-Pythia6-evtgen/crab_BdToJpsiKPi_8TeV_OfficialMC_ext2_AODSIM_newconfig_bari_smallfiles_newkinfit/160728_181420/officialBdToPsiKpi_noPtEtaCut_MuMuPiKPAT_ntpl__0000.root");
	}
      } else if ( official ) {

	if ( genericMC ) {
	  //dataset->Add("/lustre/cms/store/user/lecriste/june2016/B0ToPsiMuMu_2MuPtEtaFilter_8TeV-pythia6-evtgen/crab_official_MC_B0ToPsiMuMu_nB0/160529_225236/officialBdToPsiMuMu_noPtEtaCut_MuMuPiKPAT_ntpl.root");
	  dataset->Add("/lustre/cms/store/user/lecriste/june2016/B0ToPsiMuMu_2MuPtEtaFilter_8TeV-pythia6-evtgen/crab_official_MC_B0ToPsiMuMu_nB0/160529_225236/officialBdToPsiMuMu_noPtEtaCut_MuMuPiKPAT_ntpl__0004.root");
	}
	else if ( psi_nS.EqualTo("psi2S") ) {
	  dataset->SetDirectory("/mkcands");
	  // first official sample (12M request)
	  //dataset->Add("/lustre/cms/store/user/lecriste/march2016/BdToPsi2SKPi_NoEtaCut_MSEL5_TuneZ2star_8TeV-pythia6-evtgen/crab_official_MC_noPtEtaCuts/160324_102432/officialBdToPsiKpi_noPtEtaCuts_MuMuPiKPAT.root"); // wrong ntuple filling
	  //dataset->Add("/lustre/cms/store/user/lecriste/march2016/BdToPsi2SKPi_NoEtaCut_MSEL5_TuneZ2star_8TeV-pythia6-evtgen/crab_official_MC_noPtEtaCutsi_nMCB0/160326_202410/officialBdToPsiKpi_noPtEtaCut_MuMuPiKPAT_97percent.root"); // lustre
	  dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/officialBdToPsiKpi_noPtEtaCut_MuMuPiKPAT_97percent.root"); // cmshome copy
	  // 12M extension
	  //dataset->Add("/lustre/cms/store/user/lecriste/may2016/BdToPsi2SKPi_NoEtaCut_MSEL5_TuneZ2star_8TeV-pythia6-evtgen/crab_official_MC_noPtEtaCuts_nMCB0_ext1/160501_033217/officialBdToPsiKpi_noPtEtaCut_MuMuPiKPAT_ntpl_0000.root"); // lustre
	  dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/officialBdToPsiKpi_noPtEtaCut_MuMuPiKPAT_ext1_12M.root"); // cmshome copy

	  if ( !noPtEtaCuts ) {
	    //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/sanjay/officialBdToPsiKpi_18Mar_MuMuPiKPAT_10k.root");
	    //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/sanjay/officialBdToPsiKpi_18Mar_MuMuPiKPAT_1M.root");
	    //dataset->SetDirectory("");
	    //dataset->Add("/lustre/cms/store/user/lecriste/april2015/BdToPsi2SKPi_MSEL5_TuneZ2star_8TeV-pythia6/crab_official_MC/150425_161612/official_BdToPsiKpi_18Mar_MuMuPiKPAT.root");
	    //dataset->Add("/lustre/cms/store/user/lecriste/april2015/BdToPsi2SKPi_MSEL5_TuneZ2star_8TeV-pythia6/crab_official_MC_withGenPart/150429_142204/official_BdToPsiKpi_18Mar_MuMuPiKPAT.root");
	    //
	    //dataset->Add("../officialBdToPsiKpi_18Mar_MuMuPiKPAT_150k.root");
	    dataset->Add("/lustrehome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/officialBdToPsiKpi_MuMuPiKPAT_first4Mrequest.root"); // first offcial 4M request
	    // without dir
	    dataset->SetDirectory("");
	    //dataset->Add("/lustre/cms/store/user/lecriste/april2015/BdToPsi2SKPi_MSEL5_TuneZ2star_8TeV-pythia6/crab_official_MC_withGenVert/150511_225852/official_BdToPsiKpi_18Mar_MuMuPiKPAT.root");
	    //dataset->Add("/lustre/cms/store/user/lecriste/april2015/BdToPsi2SKPi_MSEL5_TuneZ2star_8TeV-pythia6/crab_official_MC_withCosAlpha3D/150616_232452/official_BdToPsiKpi_18Mar_MuMuPiKPAT.root");
	    // extension
	    //dataset->Add("/lustre/cms/store/user/lecriste/april2015/BdToPsi2SKPi_MSEL5_TuneZ2star_8TeV-pythia6/crab_official_MC_withGenMuons/150712_142925/official_BdToPsiKpi_18Mar_MuMuPiKPAT_ext1.root");
	    //dataset->Add("/lustre/cms/store/user/lecriste/april2015/BdToPsi2SKPi_MSEL5_TuneZ2star_8TeV-pythia6/crab_official_MC_ext1_Legnaro/150709_161215/official_BdToPsiKpi_18Mar_MuMuPiKPAT_ext1.root");
	    //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/officialBdToPsiKpi_nMCB0.root");
	    dataset->Add("/lustrehome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/officialBdToPsiKpi_nMCB0.root");
	    // with dir
	    //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/sanjay/officialBdToPsiKpi_18Mar_MuMuPiKPAT_ntpl.root");
	    //dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/original/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/officialBdToPsiKpi_inclusiveCCbarTriggers_nMuMu.root");	    
	  }
	} else if ( psi_nS.EqualTo("JPsi") ) {
	  /*
	  // filled with "nB0 > 0" instead of "nMCB0 > 0"
	  dataset->SetDirectory("");
	  dataset->Add("/lustre/cms/store/user/nsur/Jpsi_8TeV_OfficialMC_small_ntuples/merged_files_official/officialBdToJpsiKpi_MuMuPiKPAT_small_ntpls_merged.root"); // first 4M request
	  dataset->Add("/lustre/cms/store/user/nsur/Jpsi_8TeV_OfficialMC_small_ntuples/merged_files_official/officialBdToJpsiKpi_MuMuPiKPAT_ext_small_ntpls_merged_wofile73.root"); // first 16M extension
	  */
	  /*
	  // filled with "nMCB0 > 0"
	  dataset->SetDirectory("");
	  TString path = "/lustre/cms/store/user/nsur/Jpsi_8TeV_OfficialMC_small_ntuples/merged_files_official_newconfig/";
	  dataset->Add(path+"officialBdToPsiKpi_noPtEtaCut_MuMuPiKPAT_small_ntpls_merged.root"); // first 4M request
	  dataset->Add(path+"officialBdToPsiKpi_noPtEtaCut_MuMuPiKPAT_ext1_small_ntpls_merged.root"); // first 16M extension
	  dataset->Add(path+"officialBdToPsiKpi_noPtEtaCut_MuMuPiKPAT_ext2_small_ntpls_merged.root"); // second 16M extension
	  */
	  // with B0 mass constrained kinematic fit
	  TString path = "/lustre/cms/store/user/nsur/Jpsi_8TeV_OfficialMC_small_ntuples/merged_files_official_newconfig_newkinfit/"; TString filename = "officialBdToJpsiKpi_MuMuPiKPAT_newconfig_small_ntpls_newkinfit_merged";
	  dataset->SetDirectory("");
	  dataset->Add(path+filename+"_4M.root");
	  dataset->SetDirectory("/mkcands");
	  
	  dataset->Add(path+filename+"_ext1_16M.root");
	  dataset->Add(path+filename+"_ext2_16M.root");
	  dataset->Add(path+filename+"_ext3_10M.root");
	  dataset->Add(path+filename+"_ext4_20M.root");
	  
	  if ( !noPtEtaCuts ) {
	    // from B0 -> psi + X -> MuMu + ...
	    dataset->Add("/lustre/cms/store/user/lecriste/june2016/B0ToPsiMuMu_2MuPtEtaFilter_8TeV-pythia6-evtgen/crab_official_MC_B0ToPsiMuMu_nMCB0/161003_174643/officialBdToPsiKpi_18Mar_MuMuPiKPAT_ntpl_.root");
	  }
	}
      } // if (official)
    } else {
      if ( psi_nS.EqualTo("psi2S") ) {
	// dataset A (288886 events)
	if (dsA) {
	  //dataset->Add("/cmshome/pompili/MuOnia-2013/inclusive_search/PsiPrimePi/kai-rootuples-october2013/mynewntuple/reduced2012A.root");
	  // on /lustre
	  //dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012A_13Jul_JPsiPiPiPAT-full_datasetA.root");    
	  // on /cmshome
	  dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012A_13Jul_JPsiPiPiPAT-full_datasetA.root");
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
	  //dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012B_13Jul_JPsiPiPiPAT-datasetB.root");    
	  //
	  //dataset->Add("/lustre/cms/store/user/lecriste/full_datasetB/MuOniaRun2012B_13Jul_JPsiPiPiPAT_ntpl_1-499.root");
	  //dataset->Add("/lustre/cms/store/user/lecriste/full_datasetB_2/MuOniaRun2012B_13Jul_JPsiPiPiPAT-datasetB_2.root");
	  // on /cmshome
	  dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012B_13Jul_JPsiPiPiPAT-datasetB.root");
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
	  //dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012C_13Jul_JPsiPiPiPAT-full_datasetC_no3074.root");
	  //dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012C_13Jul_JPsiPiPiPAT_ntpl_3074.root");
	  // on /cmshome
	  dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012C_13Jul_JPsiPiPiPAT-full_datasetC_no3074.root");
	  dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012C_13Jul_JPsiPiPiPAT_ntpl_3074.root");
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
	  //dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012D_13Jul_JPsiPiPiPAT-datasetD_no856and1118and2110and3411.root");
	  //dataset->Add("/lustre/cms/store/user/lecriste/merged-june2014/MuOniaRun2012D_13Jul_JPsiPiPiPAT-datasetD_856and1118and2110and3411.root");
	  // on /cmshome
	  dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012D_13Jul_JPsiPiPiPAT-datasetD_856and1118and2110and3411.root");
	  dataset->Add("/cmshome/cristella/work/Z_analysis/exclusive/CMSSW_5_3_7_patch5/src/UserCode/PsiPrimePiKPAT/test/rootuples/june2014/MuOniaRun2012D_13Jul_JPsiPiPiPAT-datasetD_no856and1118and2110and3411.root");
	}
	/////////////////////////////////////////////////////////////////////////
	// test
	//dataset->Add("/cmshome/pompili/for-nairit-new/merged_MuOniaRun2012C_13Jul_JPsiPiPiPAT-datasetA_just9_random_files.root");
	/////////////////////////////////////////////////////////////////////////
      } else if ( psi_nS.EqualTo("JPsi") ) {
	TString path = "/lustre/cms/store/user/nsur/Jpsi_8TeV_data_small_ntuples/" ;
	if (B0massC.EqualTo("B0massC")) {
	  path = "/lustre/cms/store/user/nsur/MuOnia/";
	  dataset->SetDirectory("/mkcands");
	}
	if (dsA) {
	  if (!B0massC.EqualTo("B0massC")) {
	    dataset->Add(path+"MuOniaRun2012A_29Dec_MuMuKPiPAT_ntpl_merged_0000_1-818.root");
	  } else {
	    dataset->Add(path+"crab_psi1S_MuOnia2012A_AOD_B0MassConstraint_corrected/170204_051338/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl_.root");
	  }
	}
	if (dsB) {
	  if (!B0massC.EqualTo("B0massC")) {
	    dataset->Add(path+"MuOniaRun2012B_08Oct_MuMuKPiPAT_ntpl_merged_0000_1-500.root");
	    dataset->Add(path+"MuOniaRun2012B_08Oct_MuMuKPiPAT_ntpl_merged_0000_501-999.root");
	    dataset->Add(path+"MuOniaRun2012B_08Oct_MuMuKPiPAT_ntpl_merged_0000_1000-1999.root");
	    dataset->Add(path+"MuOniaRun2012B_08Oct_MuMuKPiPAT_ntpl_merged_0000_2000-2546.root");
	  } else {
	    dataset->Add(path+"crab_psi1S_MuOnia2012B_AOD_B0MassConstraint_corrected/170208_105409/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl_.root");
	    dataset->Add(path+"crab_psi1S_MuOnia2012B_AOD_B0MassConstraint_corrected_resub_failedjobs/170217_055814/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl_.root");
	  }
	}
	if (dsC) {
	  if (!B0massC.EqualTo("B0massC")) {
	    dataset->Add(path+"MuOniaRun2012C_31Dec_MuMuKPiPAT_ntpl_merged_0000_1-999.root");
	    dataset->Add(path+"MuOniaRun2012C_29Dec_MuMuKPiPAT_ntpl_merged_0001_1000-1999.root");
	    dataset->Add(path+"MuOniaRun2012C_31Dec_MuMuKPiPAT_ntpl_merged_0002_2000-2015.root");
	  } else {
	    //dataset->Add(path+"crab_psi1S_MuOnia2012C_AOD_B0MassConstraint_corrected/170208_105510/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl_.root");
	    dataset->Add(path+"crab_psi1S_MuOnia2012C_AOD_B0MassConstraint_corrected/170208_105510/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0000.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012C_AOD_B0MassConstraint_corrected/170208_105510/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0001.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012C_AOD_B0MassConstraint_corrected/170208_105510/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0002.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012C_AOD_B0MassConstraint_corrected/170208_105510/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0003.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012C_AOD_B0MassConstraint_corrected/170208_105510/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0004.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012C_AOD_B0MassConstraint_corrected/170208_105510/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0005.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012C_AOD_B0MassConstraint_corrected/170208_105510/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0006.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012C_AOD_B0MassConstraint_corrected/170208_105510/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0007.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012C_AOD_B0MassConstraint_corrected/170208_105510/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0008.root");
	    //
	    dataset->Add(path+"crab_psi1S_MuOnia2012C_AOD_B0MassConstraint_corrected_resub_failedjobs/170216_133924/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl_.root");
	  }
	}
	if (dsD) {
	  if (!B0massC.EqualTo("B0massC")) {
	    dataset->Add(path+"MuOniaRun2012D_31Dec_MuMuKPiPAT_ntpl_merged_0000_1-999.root");
	    dataset->Add(path+"MuOniaRun2012D_31Dec_MuMuKPiPAT_ntpl_merged_0001_1000-1999.root");
	    dataset->Add(path+"MuOniaRun2012D_31Dec_MuMuKPiPAT_ntpl_merged_0002_2000-2999.root");
	    dataset->Add(path+"MuOniaRun2012D_31Dec_MuMuKPiPAT_ntpl_merged_0003_3000-3718.root");
	  } else {
	    //dataset->Add(path+"crab_psi1S_MuOnia2012D_AOD_B0MassConstraint_corrected/170208_105605/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl_.root");
	    //dataset->Add(path+"crab_psi1S_MuOnia2012D_AOD_B0MassConstraint_corrected/170208_105605/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0000.root");
            //dataset->Add(path+"crab_psi1S_MuOnia2012D_AOD_B0MassConstraint_corrected/170208_105605/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0001.root");
            //dataset->Add(path+"crab_psi1S_MuOnia2012D_AOD_B0MassConstraint_corrected/170208_105605/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0002.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012D_AOD_B0MassConstraint_corrected/170208_105605/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0003.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012D_AOD_B0MassConstraint_corrected/170208_105605/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0004.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012D_AOD_B0MassConstraint_corrected/170208_105605/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0005.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012D_AOD_B0MassConstraint_corrected/170208_105605/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0006.root");
            dataset->Add(path+"crab_psi1S_MuOnia2012D_AOD_B0MassConstraint_corrected/170208_105605/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl__0007.root");
	    //
	    dataset->Add(path+"crab_psi1S_MuOnia2012D_AOD_B0MassConstraint_corrected_resub_failedjobs/170216_061829/MuOniaRun2012C_25Apr_MuMuPiPiPAT_ntpl_.root");
	  }
	}
      } // if ( psi_nS.EqualTo("JPsi") )

    } // if ( !MC )
    //
    //
    gEnv->SetValue("Proof.StatsHist",1);
    gEnv->SetValue("Proof.StatsTrace",1);
    gEnv->SetValue("Proof.SlaveStatsTrace",1);
    //gEnv->SetValue("ProofServ.Sandbox", "/cmshome/cristella/work/Z_analysis/exclusive/clean_14ott/CMSSW_5_3_22/src/UserCode/MuMuPiKPAT/test/sanjay/selector/officialMC/"); 
    //
    TString selector = "psiPrimePiK_MC";
    //
    TProof *p = TProof::Open("workers=18"); // need more than 16 for J/psi data (18 enough)
    //p->Print(); return;    
    // Processing...
    cout << ">> Processing " << selector << " ..." << endl;
    TString selectorcplus = selector;
    selectorcplus+= ".C+";
    //selectorcplus+= ".C"; // not working
    TString option = "";
    if ( MC ) {
      if ( !official )  
	option = "MC_"+psi_nS;
      else
	if ( !noPtEtaCuts )
	  option = "officialMC_"+psi_nS;
	else {
	  if ( !genericMC )
	    option = "officialMC_noPtEtaCuts_"+psi_nS;
	  else
	    option = "officialMC_generic";
	}
    } else
      option = "Data_"+psi_nS ;

    if (B0massC.EqualTo("B0massC"))
      option.Append("_"+B0massC);

    cout <<"Running with option \"" <<option <<"\"" <<endl;
    p->Process(dataset, selectorcplus, option);  
    
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
