#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/RecoAlgos/interface/TrackFullCloneSelectorBase.h"
#include "ForwardAnalysis/Utilities/interface/TrackAnalysisSelector.h"

namespace reco { 
  namespace modules {

    typedef TrackFullCloneSelectorBase<forwardAnalysis::TrackAnalysisSelector> TrackAnalysisSelector;

    DEFINE_FWK_MODULE(TrackAnalysisSelector);
  }
}
