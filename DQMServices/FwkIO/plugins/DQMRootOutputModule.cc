// -*- C++ -*-
//
// Package:     FwkIO
// Class  :     DQMRootOutputModule
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  Chris Jones
//         Created:  Fri Apr 29 13:26:29 CDT 2011
// $Id$
//

// system include files
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <boost/shared_ptr.hpp>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

// user include files
#include "FWCore/Framework/interface/OutputModule.h"
#include "FWCore/Framework/interface/RunPrincipal.h"
#include "FWCore/Framework/interface/LuminosityBlockPrincipal.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "format.h"

namespace {
  class TreeHelperBase {
  public:
    TreeHelperBase(): m_wasFilled(false), m_firstIndex(0),m_lastIndex(0) {}
    virtual ~TreeHelperBase(){}
    void fill(MonitorElement* iElement) {
      doFill(iElement); 
      if(m_wasFilled) {++m_lastIndex;} 
      m_wasFilled = true; }
    bool wasFilled() const { return m_wasFilled;}
    void getRangeAndReset(ULong64_t& iFirstIndex, ULong64_t& iLastIndex) {
      iFirstIndex = m_firstIndex;
      iLastIndex = m_lastIndex;
      m_wasFilled = false;
      m_firstIndex = m_lastIndex +1;
      m_lastIndex = m_firstIndex;
    }
  private:
    virtual void doFill(MonitorElement*) = 0;
    bool m_wasFilled;
    ULong64_t m_firstIndex;
    ULong64_t m_lastIndex;
  };
  
  template<class T>
  class TreeHelper : public TreeHelperBase {
  public:
    TreeHelper(TTree* iTree, std::string* iFullNameBufferPtr ):
     m_tree(iTree), m_flagBuffer(0),m_fullNameBufferPtr(iFullNameBufferPtr){ setup();}
     virtual void doFill(MonitorElement* iElement) {
       *m_fullNameBufferPtr = iElement->getFullname();
       m_flagBuffer = iElement->getTag();
       m_bufferPtr = dynamic_cast<T*>(iElement->getRootObject());
       assert(0!=m_bufferPtr);
       //std::cout <<"#entries: "<<m_bufferPtr->GetEntries()<<std::endl;
       m_tree->Fill();
     }
     
     
  private:
    void setup() {
      m_tree->Branch(kFullNameBranch,&m_fullNameBufferPtr);
      m_tree->Branch(kFlagBranch,&m_flagBuffer);
      
      m_bufferPtr = 0;
      m_tree->Branch(kValueBranch,&m_bufferPtr,16*1024,0);
    }
    TTree* m_tree;
    uint32_t m_flagBuffer;
    std::string* m_fullNameBufferPtr;
    T* m_bufferPtr;
  };
  
  class IntTreeHelper: public TreeHelperBase {
  public:
    IntTreeHelper(TTree* iTree, std::string* iFullNameBufferPtr):
     m_tree(iTree), m_flagBuffer(0),m_fullNameBufferPtr(iFullNameBufferPtr)
     {setup();}

    virtual void doFill(MonitorElement* iElement) {
     *m_fullNameBufferPtr = iElement->getFullname();
     m_flagBuffer = iElement->getTag();
     m_buffer = iElement->getIntValue();
     m_tree->Fill();
    }

  private:
    void setup() {
      m_tree->Branch(kFullNameBranch,&m_fullNameBufferPtr);
      m_tree->Branch(kFlagBranch,&m_flagBuffer);
      m_tree->Branch(kValueBranch,&m_buffer);
    }
    TTree* m_tree;
    uint32_t m_flagBuffer;
    std::string* m_fullNameBufferPtr;
    Long64_t m_buffer;
  };

  class FloatTreeHelper: public TreeHelperBase {
  public:
    FloatTreeHelper(TTree* iTree, std::string* iFullNameBufferPtr):
     m_tree(iTree), m_flagBuffer(0),m_fullNameBufferPtr(iFullNameBufferPtr)
     {setup();}
   virtual void doFill(MonitorElement* iElement) {
     *m_fullNameBufferPtr = iElement->getFullname();
     m_flagBuffer = iElement->getTag();
     m_buffer = iElement->getFloatValue();
     m_tree->Fill();
   }
  private:
    void setup() {
      m_tree->Branch(kFullNameBranch,&m_fullNameBufferPtr);
      m_tree->Branch(kFlagBranch,&m_flagBuffer);
      m_tree->Branch(kValueBranch,&m_buffer);
    }
    
    TTree* m_tree;
    uint32_t m_flagBuffer;
    std::string* m_fullNameBufferPtr;
    double m_buffer;
  };

  class StringTreeHelper: public TreeHelperBase {
  public:
    StringTreeHelper(TTree* iTree, std::string* iFullNameBufferPtr):
     m_tree(iTree), m_flagBuffer(0),m_fullNameBufferPtr(iFullNameBufferPtr), m_bufferPtr(&m_buffer)
     {setup();}
   virtual void doFill(MonitorElement* iElement) {
     *m_fullNameBufferPtr = iElement->getFullname();
     m_flagBuffer = iElement->getTag();
     m_buffer = iElement->getStringValue();
     m_tree->Fill();
   }
  private:
    void setup() {
      m_tree->Branch(kFullNameBranch,&m_fullNameBufferPtr);
      m_tree->Branch(kFlagBranch,&m_flagBuffer);
      m_tree->Branch(kValueBranch,&m_bufferPtr);
    }
    
    TTree* m_tree;
    uint32_t m_flagBuffer;
    std::string* m_fullNameBufferPtr;
    TString m_buffer;
    TString* m_bufferPtr;
  };

}


class DQMRootOutputModule : public edm::OutputModule {
public:
  // We do not take ownership of passed stream.
  explicit DQMRootOutputModule(edm::ParameterSet const& pset);
  virtual ~DQMRootOutputModule();
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void write(edm::EventPrincipal const& e);
  virtual void writeLuminosityBlock(edm::LuminosityBlockPrincipal const&);
  virtual void writeRun(edm::RunPrincipal const&);
  virtual void beginRun(edm::RunPrincipal const& r);
  std::string m_fileName;
  std::auto_ptr<TFile> m_file;
  std::vector<boost::shared_ptr<TreeHelperBase> > m_treeHelpers;
  
  unsigned int m_run;
  unsigned int m_lumi;
  unsigned int m_type;
  ULong64_t m_firstIndex;
  ULong64_t m_lastIndex;
  
  std::string m_fullNameBuffer;
  std::string* m_fullNameBufferPtr;
  std::map<unsigned int, unsigned int> m_dqmKindToTypeIndex;
  TTree* m_indicesTree;
  
};

//
// constants, enums and typedefs
//

static TreeHelperBase*
makeHelper(unsigned int iTypeIndex,
           TTree* iTree, 
           std::string* iFullNameBufferPtr) {
  switch(iTypeIndex) {
    case kIntIndex:
    return new IntTreeHelper(iTree,iFullNameBufferPtr);
    case kFloatIndex:
    return new FloatTreeHelper(iTree,iFullNameBufferPtr);
    case kStringIndex:
    return new StringTreeHelper(iTree,iFullNameBufferPtr);
    case kTH1FIndex:
    return new TreeHelper<TH1F>(iTree,iFullNameBufferPtr);
    case kTH1SIndex:
    return new TreeHelper<TH1S>(iTree,iFullNameBufferPtr);
    case kTH1DIndex:
    return new TreeHelper<TH1D>(iTree,iFullNameBufferPtr);
    case kTH2FIndex:
    return new TreeHelper<TH2F>(iTree,iFullNameBufferPtr);
    case kTH2SIndex:
    return new TreeHelper<TH2S>(iTree,iFullNameBufferPtr);
    case kTH2DIndex:
    return new TreeHelper<TH2D>(iTree,iFullNameBufferPtr);
    case kTH3FIndex:
    return new TreeHelper<TH3F>(iTree,iFullNameBufferPtr);
    case kTProfileIndex:
    return new TreeHelper<TProfile>(iTree,iFullNameBufferPtr);
    case kTProfile2DIndex:
    return new TreeHelper<TProfile2D>(iTree,iFullNameBufferPtr);
  }
  assert(false);
  return 0;
}

//
// static data member definitions
//

//
// constructors and destructor
//
DQMRootOutputModule::DQMRootOutputModule(edm::ParameterSet const& pset):
edm::OutputModule(pset),
m_fileName(pset.getUntrackedParameter<std::string>("fileName")),
m_file(0),
m_treeHelpers(kNIndicies,boost::shared_ptr<TreeHelperBase>()),
m_fullNameBufferPtr(&m_fullNameBuffer)
{
  //NOTE: I need to also set the I/O performance settings
  m_file = std::auto_ptr<TFile>(new TFile(m_fileName.c_str(),"CREATE"));  


  m_indicesTree = new TTree(kIndicesTree,kIndicesTree);
  m_indicesTree->Branch(kRunBranch,&m_run);
  m_indicesTree->Branch(kLumiBranch,&m_lumi);
  m_indicesTree->Branch(kTypeBranch,&m_type);
  m_indicesTree->Branch(kFirstIndex,&m_firstIndex);
  m_indicesTree->Branch(kLastIndex,&m_lastIndex);
  m_indicesTree->SetDirectory(m_file.get());
  
  unsigned int i = 0;
  for(std::vector<boost::shared_ptr<TreeHelperBase> >::iterator it = m_treeHelpers.begin(), itEnd = m_treeHelpers.end();
  it != itEnd;
  ++it,++i) {
    //std::cout <<"making "<<kTypeNames[i]<<std::endl;
    TTree* tree = new TTree(kTypeNames[i],kTypeNames[i]);
    *it = boost::shared_ptr<TreeHelperBase>(makeHelper(i,tree,m_fullNameBufferPtr));
    tree->SetDirectory(m_file.get()); //TFile takes ownership
  }
  
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_INT]=kIntIndex;
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_REAL]=kFloatIndex;
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_STRING]=kStringIndex;
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_TH1F]=kTH1FIndex;
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_TH1S]=kTH1SIndex;
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_TH1D]=kTH1DIndex;
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_TH2F]=kTH2FIndex;
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_TH2S]=kTH2SIndex;
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_TH2D]=kTH2DIndex;
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_TH3F]=kTH3FIndex;
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_TPROFILE]=kTProfileIndex;
  m_dqmKindToTypeIndex[MonitorElement::DQM_KIND_TPROFILE2D]=kTProfile2DIndex;
}

// DQMRootOutputModule::DQMRootOutputModule(const DQMRootOutputModule& rhs)
// {
//    // do actual copying here;
// }

DQMRootOutputModule::~DQMRootOutputModule()
{
  m_file->Write();
  m_file->Close();
}

//
// assignment operators
//
// const DQMRootOutputModule& DQMRootOutputModule::operator=(const DQMRootOutputModule& rhs)
// {
//   //An exception safe implementation is
//   DQMRootOutputModule temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
void 
DQMRootOutputModule::write(edm::EventPrincipal const& ){
  
}
void 
DQMRootOutputModule::writeLuminosityBlock(edm::LuminosityBlockPrincipal const& iLumi) {
  edm::Service<DQMStore> dstore;
  m_run=iLumi.id().run();
  m_lumi = iLumi.id().value();
  
  std::vector<MonitorElement *> items(dstore->getAllContents(""));
  for(std::vector<MonitorElement*>::iterator it = items.begin(), itEnd=items.end();
      it!=itEnd;
      ++it) {
    if((*it)->getLumiFlag()) {
      std::map<unsigned int,unsigned int>::iterator itFound = m_dqmKindToTypeIndex.find((*it)->kind());
      assert(itFound !=m_dqmKindToTypeIndex.end());
      m_treeHelpers[itFound->second]->fill(*it);
    }
  }
  //Now store the relationship between run/lumi and indices in the other TTrees
  unsigned int typeIndex = 0;
  for(std::vector<boost::shared_ptr<TreeHelperBase> >::iterator it = m_treeHelpers.begin(), itEnd = m_treeHelpers.end();
      it != itEnd;
      ++it,++typeIndex) {
    if((*it)->wasFilled()) {
      m_type = typeIndex;
      (*it)->getRangeAndReset(m_firstIndex,m_lastIndex);
      m_indicesTree->Fill();
    }
  }
}

void DQMRootOutputModule::writeRun(edm::RunPrincipal const& iRun){
  edm::Service<DQMStore> dstore;
  m_run=iRun.id().run();
  m_lumi = 0;
  
  std::vector<MonitorElement *> items(dstore->getAllContents(""));
  for(std::vector<MonitorElement*>::iterator it = items.begin(), itEnd=items.end();
      it!=itEnd;
      ++it) {
    if(not (*it)->getLumiFlag()) {
      std::map<unsigned int,unsigned int>::iterator itFound = m_dqmKindToTypeIndex.find((*it)->kind());
      assert  (itFound !=m_dqmKindToTypeIndex.end());
      m_treeHelpers[itFound->second]->fill(*it);
    }
  }
  
  //Now store the relationship between run/lumi and indices in the other TTrees
  unsigned int typeIndex = 0;
  for(std::vector<boost::shared_ptr<TreeHelperBase> >::iterator it = m_treeHelpers.begin(), itEnd = m_treeHelpers.end();
      it != itEnd;
      ++it,++typeIndex) {
    if((*it)->wasFilled()) {
      m_type = typeIndex;
      (*it)->getRangeAndReset(m_firstIndex,m_lastIndex);
      m_indicesTree->Fill();
    }
  }
  
}

void DQMRootOutputModule::beginRun(edm::RunPrincipal const& ) {
  edm::Service<DQMStore> dstore;
  
  std::vector<MonitorElement *> items(dstore->getAllContents(""));
  for(std::vector<MonitorElement*>::iterator it = items.begin(), itEnd=items.end();
      it!=itEnd;
      ++it) {
        //NOTE: the following logic comes from MEtoEDMConverter
        switch((*it)->kind()) {
          case MonitorElement::DQM_KIND_INT:
          case MonitorElement::DQM_KIND_REAL:
          case MonitorElement::DQM_KIND_STRING:
            break;
          default:
            (*it)->Reset();
      }
  }
  
}
//
// const member functions
//

//
// static member functions
//
void
DQMRootOutputModule::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


DEFINE_FWK_MODULE(DQMRootOutputModule);