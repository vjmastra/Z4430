from WMCore.Configuration import Configuration
config = Configuration()

config.section_('General')
config.General.transferOutputs = True

# Data
#config.General.requestName = 'dataset_C'
#config.General.requestName = 'dataset_C_missingLumis' # for resubmission
#config.General.requestName = 'dataset_C_missingLumis_backup'
#config.General.requestName = 'psi1and2S_dataset_D' # wrong J/psi triggers spelling
#config.General.requestName = 'psi1SokAnd2S_dataset_D'
#config.General.requestName = 'psi1SokAnd2S_dataset_C'
#config.General.requestName = 'psi1SokAnd2S_dataset_C_backup'
#config.General.requestName = 'psi1SokAnd2S_dataset_C_Pisa'
#config.General.requestName = 'psi1SokAnd2S_dataset_B_backup'
#config.General.requestName = 'B0toMuMuPP_datasetB'
#config.General.requestName = 'B0toMuMuPP_datasetC'
#config.General.requestName = 'B0toMuMuPP_datasetD'
#config.General.requestName = 'B0toMuMuPP_datasetD_withJpsiInclTrigg_nB0'
#config.General.requestName = 'B0toMuMuPP_datasetC_withJpsiInclTrigg_nB0'
#config.General.requestName = 'B0toMuMuPP_datasetB_withJpsiInclTrigg_nB0'
# MC
#config.General.requestName = 'official_MC'
#config.General.requestName = 'official_MC_withGenPart'
#config.General.requestName = 'official_MC_withGenVert'
#config.General.requestName = 'official_MC_withCosAlpha3D'
#config.General.requestName = 'official_MC_ext1_Legnaro'
#config.General.requestName = 'official_MC_withGenMuons'
#config.General.requestName = 'official_MC_nMCB0TreeFilling'
#config.General.requestName = 'official_MC_nMCB0ToJPsiPPbar'
#config.General.requestName = 'official_MC_noPtEtaCuts'
#config.General.requestName = 'official_MC_noPtEtaCutsi_nMCB0' # used with T2_IT_Bari
config.General.requestName = 'official_MC_noPtEtaCuts_nMCB0' # used with T2_CH_CERN
config.General.requestName = 'official_MC_noPtEtaCuts_nMCB0_ext1'
#config.General.requestName = 'official_MC_first4Mrequest_nMCB0' 
# Generic MC
config.General.requestName = 'official_MC_B0ToPsiMuMu_nB0'
config.General.requestName = 'official_MC_Bs0ToPsiMuMu_nB0'
config.General.requestName = 'official_MC_Bs0ToPsiMuMu_nB0_recoveryTask'
config.General.requestName = 'official_MC_Bu0ToPsiMuMu_nB0'
config.General.requestName = 'official_MC_LambdaBToPsiMuMu_nB0'
config.General.requestName = 'official_MC_B0ToPsiMuMu_nMCB0'

#config.General.transferLogs = True
config.General.transferLogs = False

config.section_('JobType')
#config.JobType.psetName = 'runMuMuPiKPAT_dataOrMC_03Mar.py'
config.JobType.psetName = 'runMuMuPiKPAT_dataOrMC_09Apr.py'

config.JobType.pluginName = 'Analysis'

config.section_('Data')
#config.Data.inputDataset = '/MuOniaParked/Run2012B-22Jan2013-v1/AOD'
#config.Data.inputDataset = '/MuOniaParked/Run2012C-22Jan2013-v1/AOD'
#config.Data.inputDataset = '/MuOniaParked/Run2012D-22Jan2013-v1/AOD'
#config.Data.inputDataset = '/BdToPsi2SKPi_MSEL5_TuneZ2star_8TeV-pythia6/Summer12DR53X-PU_RD2_START53_V19F-v1/AODSIM'
#config.Data.inputDataset = '/BdToPsi2SKPi_MSEL5_TuneZ2star_8TeV-pythia6/Summer12DR53X-PU_RD2_START53_V19F_ext1-v1/AODSIM'
#config.Data.inputDataset = '/B0ToJPsiPPbar_MSEL5_TuneZ2star_8TeV-pythia6-evtgen/Summer12DR53X-PU_RD2_START53_V19F-v1/AODSIM'
#config.Data.inputDataset = '/BdToPsi2SKPi_NoEtaCut_MSEL5_TuneZ2star_8TeV-pythia6-evtgen/Summer12DR53X-PU_RD1_START53_V7N-v1/AODSIM'
#config.Data.inputDataset = '/BdToPsi2SKPi_NoEtaCut_MSEL5_TuneZ2star_8TeV-pythia6-evtgen/Summer12DR53X-PU_S10_START53_V19_ext1-v1/AODSIM'
# Generic MC
config.Data.inputDataset = '/B0ToPsiMuMu_2MuPtEtaFilter_8TeV-pythia6-evtgen/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM'
#config.Data.inputDataset = '/BsToPsiMuMu_2MuPtEtaFilter_8TeV-pythia6-evtgen/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM'
#config.Data.inputDataset = '/BpToPsiMuMu_2MuPtEtaFilter_8TeV-pythia6-evtgen/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM'
#config.Data.inputDataset = '/LambdaBToPsiMuMu_2MuPtEtaFilter_8TeV-pythia6-evtgen/Summer12_DR53X-PU_S10_START53_V7A-v2/AODSIM'

# Data
#config.Data.splitting = 'LumiBased'
#
#config.Data.lumiMask = 'Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON_MuonPhys.txt'
#config.Data.unitsPerJob = 6 # 6: Task consists of 9147 jobs to process ? lumis for datasetB; 10: Task consists of 7687 jobs to process 76868 lumis for datasetD; Task consists of 8115 jobs to process 81147 lumis for datasetC

#config.Data.lumiMask = 'crab_dataset_C/results/missingLumiSummary.json' # for resubmission
#config.Data.unitsPerJob = 1 # for resubmission

# MC
#config.Data.splitting = 'FileBased'
#config.Data.unitsPerJob = 1 # 365 (= number of files) jobs created... FileBased is the forced choice since next one doesn't work
##config.Data.splitting = 'EventAwareLumiBased'
##config.Data.unitsPerJob = 10 # not working
#config.Data.lumiMask = 'crab_official_MC_Bs0ToPsiMuMu_nB0/results/failedLumis.json'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 46

config.Data.totalUnits = -1

config.Data.publication = False

#config.Data.outLFNDirBase = '/store/user/lecriste/april2015/'
#config.Data.outLFNDirBase = '/store/user/lecriste/march2016/'
#config.Data.outLFNDirBase = '/store/user/lecriste/may2016/'
config.Data.outLFNDirBase = '/store/user/lecriste/june2016/'
#config.Data.outLFNDirBase = '/store/user/lecriste/pentaQuark/'
#config.Data.outLFNDirBase = '/store/group/phys_muon/lecriste/TnP/' # for CERN
#/eos/cms/store/group/phys_muon/lecriste/TnP

#config.Data.ignoreLocality = True
config.Data.ignoreLocality = False # for resubmission

config.section_('User')

config.section_('Site')
#config.Site.blacklist = ['T0', 'T1'] # T0 blacklisted by default
config.Site.blacklist = ['T1*']
#config.Site.blacklist = ['T1*','T2_US_UCSD','T3_UK_London_QMUL','T2_US_Vanderbilt','T2_HU_Budapest','T3_US_TTU']

config.Site.storageSite = 'T2_IT_Bari'
#config.Site.storageSite = 'T2_IT_Legnaro'
#config.Site.storageSite = 'T2_CH_CERN'
#config.Site.storageSite = 'T2_IT_Pisa'

