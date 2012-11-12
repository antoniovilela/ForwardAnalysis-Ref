#include "Utilities/AnalysisTools/interface/TrackOutsideJetsSelector.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Math/interface/deltaR.h"

using namespace reco;
using analysisTools::TrackOutsideJetsSelector;

TrackOutsideJetsSelector::TrackOutsideJetsSelector(const edm::ParameterSet& pset):
   jetTag_(pset.getParameter<edm::InputTag>("JetTag")),
   coneSize_(pset.getParameter<double>("JetConeSize")){}

TrackOutsideJetsSelector::~TrackOutsideJetsSelector() {}

bool TrackOutsideJetsSelector::operator()(const reco::Track& track, const edm::Event& event) const {
   edm::Handle<edm::View<Candidate> > jetCollectionH;
   event.getByLabel(jetTag_,jetCollectionH);
   const edm::View<Candidate>& jetColl = *(jetCollectionH.product());
 
   bool associated = false;
   for(edm::View<Candidate>::const_iterator jet = jetColl.begin(); jet != jetColl.end(); ++jet){
      //FIXME: use jet-tracks association
      if(deltaR(jet->eta(),jet->phi(),track.eta(),track.phi()) < coneSize_) associated = true;
   }

   bool accept = !associated;
  
   return accept;
}
