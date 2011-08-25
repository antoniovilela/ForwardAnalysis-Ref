#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/RecoAlgos/interface/TrackFullCloneSelectorBase.h"
#include "ForwardAnalysis/Utilities/interface/TrackOutsideJetsSelector.h"

namespace reco { 
  namespace modules {

    typedef TrackFullCloneSelectorBase<forwardAnalysis::TrackOutsideJetsSelector> TrackOutsideJetsSelector;

    DEFINE_FWK_MODULE(TrackOutsideJetsSelector);
  }
}
