{
if (gSystem->Exec("rm psiPrimePiK_MC_C.*"))
  cout <<"WARNING! psiPrimePiK_MC_C.* not removed" <<endl ;
gInterpreter->AddIncludePath("/cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/roofit/5.34.09-cms5/include");
gROOT->ProcessLine(".x Run.prooflite_ZInclAnalysis.C");
}
