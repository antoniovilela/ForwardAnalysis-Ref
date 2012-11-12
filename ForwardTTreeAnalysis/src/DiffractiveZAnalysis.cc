
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZAnalysis.h"


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "FWCore/Utilities/interface/RegexMatch.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

//using namespace forwardAnalysis;
using diffractiveZAnalysis::DiffractiveZAnalysis;

const char* DiffractiveZAnalysis::name = "DiffractiveZAnalysis";

DiffractiveZAnalysis::DiffractiveZAnalysis(const edm::ParameterSet& pset):
  triggerResultsTag_(pset.getParameter<edm::InputTag>("TriggerResultsTag")),
  jetTag_(pset.getParameter<edm::InputTag>("jetTag"))
{
}

DiffractiveZAnalysis::~DiffractiveZAnalysis(){}

void DiffractiveZAnalysis::begin() {}

void DiffractiveZAnalysis::begin(const edm::Run& run, const edm::EventSetup& setup) {}

void DiffractiveZAnalysis::end() {}

void DiffractiveZAnalysis::fill(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
 

  eventData.reset();

  fillTriggerInfo(eventData,event,setup);
 
 
/*
  edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  if(jetCollectionH->size() > 0){
     const reco::Jet& leadingJet = (*jetCollectionH)[0];

     eventData.leadingJetPt_ = leadingJet.pt();
     eventData.leadingJetEta_ = leadingJet.eta();
     eventData.leadingJetPhi_ = leadingJet.phi();
  }
 */
}

// Fill Trigger

void DiffractiveZAnalysis::fillTriggerInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
 
  edm::Handle<edm::TriggerResults> triggerResults;
  event.getByLabel(triggerResultsTag_, triggerResults);

  if( triggerResults.isValid() ){
  const edm::TriggerNames& triggerNames = event.triggerNames(*triggerResults);

  size_t idxpath = 0;
  std::vector<std::string>::const_iterator hltpath = hltPathNames_.begin();
  std::vector<std::string>::const_iterator hltpaths_end = hltPathNames_.end(); 
  for(; hltpath != hltpaths_end; ++hltpath,++idxpath){
     std::string resolvedPathName; 
     if( edm::is_glob( *hltpath ) ){
        std::vector< std::vector<std::string>::const_iterator > matches = edm::regexMatch(triggerNames.triggerNames(), *hltpath);

        if( matches.empty() )
           throw cms::Exception("Configuration") << "Could not find any HLT path of type " << *hltpath << "\n";
        else if( matches.size() > 1)
           throw cms::Exception("Configuration") << "HLT path type " << *hltpath << " not unique\n";
        else resolvedPathName = *(matches[0]);
     } else{
        resolvedPathName = *hltpath;
     }

     int idx_HLT = triggerNames.triggerIndex(resolvedPathName);
     int accept_HLT = ( triggerResults->wasrun(idx_HLT) && triggerResults->accept(idx_HLT) ) ? 1 : 0;
     eventData.SetHLTPath(idxpath, accept_HLT);
     //eventData.SetHLTPathName(idxpath, resolvedPathName);
     //hltTriggerPassHisto_->Fill( (*hltpath).c_str(), 1 ); 
  }
 
}else{
        std::cout << "\n No valid trigger result." <<std::endl;
     }

}

