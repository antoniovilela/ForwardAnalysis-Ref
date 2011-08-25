#ifndef ForwardAnalysis_Utilities_TrackOutsideJetsSelector_h
#define ForwardAnalysis_Utilities_TrackOutsideJetsSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace forwardAnalysis {

class TrackOutsideJetsSelector {
   public:
      explicit TrackOutsideJetsSelector(const edm::ParameterSet&);
      ~TrackOutsideJetsSelector();
      bool operator()(const reco::Track&, const edm::Event&) const;
   private:
      edm::InputTag jetTag_;
      double coneSize_; 
};

} // namespace
#endif
