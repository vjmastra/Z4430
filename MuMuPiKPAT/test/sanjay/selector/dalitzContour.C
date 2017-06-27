#include <TGraph.h>
#include <TMath.h>

pair<TGraph*,TGraph*> dalitzContour(Float_t mMother, Float_t mX, Float_t mY, Float_t mXY, Int_t granularity) {

  const Int_t m = granularity;
  Double_t x[granularity], m23_max[granularity], m23_min[granularity];
  Double_t E2[granularity], E3[granularity];
  
  const Double_t m_dau1 = mX, m_dau2 = mY, m_dau3 = mXY; 
	    
  const Double_t m12_min = (m_dau1 + m_dau2)*(m_dau1 + m_dau2);
  const Double_t m12_max = (mMother - m_dau3)*(mMother - m_dau3);
  const Double_t step = (m12_max - m12_min)/(m-1);
  
  x[0] = m12_min + 0.0001;
	    
  for (Int_t k=1; k<m; k++ ) 
    x[k] = x[k-1] + step;
	    
  Int_t n = granularity-1;
  for (Int_t i=0; i<n; i++) {
    E2[i] = (x[i] - m_dau1*m_dau1 + m_dau2*m_dau2)/(2*sqrt(x[i]));
    E3[i] = (mMother*mMother - x[i] - m_dau3*m_dau3)/(2*sqrt(x[i]));
    m23_min[i] = (E2[i]+E3[i])*(E2[i]+E3[i]) - TMath::Power((sqrt(E2[i]*E2[i] - m_dau2*m_dau2) + sqrt(E3[i]*E3[i] - m_dau3*m_dau3)),2);
    m23_max[i] = (E2[i]+E3[i])*(E2[i]+E3[i]) - TMath::Power((sqrt(E2[i]*E2[i] - m_dau2*m_dau2) - sqrt(E3[i]*E3[i] - m_dau3*m_dau3)),2);
  }
  
  return make_pair(new TGraph(n,x,m23_min), new TGraph(n,x,m23_max));

}
