#ifndef Utilities_AnalysisTools_TrackWithJetSelector_h
#define Utilities_AnalysisTools_TrackWithJetSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace analysisTools {

class TrackWithJetSelector {
   public:
      explicit TrackWithJetSelector(const edm::ParameterSet&);
      ~TrackWithJetSelector();
      bool operator()(const reco::Track&, const edm::Event&) const;
   private:
      edm::InputTag jetTag_;
      double deltaPhiMin_;
      double deltaPhiMax_; 
};

} // namespace
#endif
