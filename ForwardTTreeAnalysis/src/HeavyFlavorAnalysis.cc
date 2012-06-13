
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/HeavyFlavorAnalysis.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/HeavyFlavorEvent.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

//using namespace forwardAnalysis;
using heavyFlavorAnalysis::HeavyFlavorAnalysis;

const char* HeavyFlavorAnalysis::name = "HeavyFlavorAnalysis";

HeavyFlavorAnalysis::HeavyFlavorAnalysis(const edm::ParameterSet& pset):
  jetTag_(pset.getParameter<edm::InputTag>("jetTag"))
{
}

HeavyFlavorAnalysis::~HeavyFlavorAnalysis(){}

void HeavyFlavorAnalysis::begin() {}

void HeavyFlavorAnalysis::begin(const edm::Run& run, const edm::EventSetup& setup) {}

void HeavyFlavorAnalysis::end() {}

void HeavyFlavorAnalysis::fill(HeavyFlavorEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  eventData.reset();

  edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  if(jetCollectionH->size() > 0){
     const reco::Jet& leadingJet = (*jetCollectionH)[0];

     eventData.leadingJetPt_ = leadingJet.pt();
     eventData.leadingJetEta_ = leadingJet.eta();
     eventData.leadingJetPhi_ = leadingJet.phi();
  }
}
