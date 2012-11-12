#ifndef Utilities_AnalysisTools_TrackAnalysisSelector_h
#define Utilities_AnalysisTools_TrackAnalysisSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace analysisTools {

class TrackAnalysisSelector {
   public:
      explicit TrackAnalysisSelector(const edm::ParameterSet&);
      ~TrackAnalysisSelector();
      bool operator()(const reco::Track&, const edm::Event&) const;
   private:
      edm::InputTag vertexTag_;

      double ptMin_;
      double etaMin_,etaMax_;
      double dxySigMax_;
      double dzSigMax_;
      double sigPtMax_;

      reco::TrackBase::TrackQuality quality_;
};

} // namespace
#endif
