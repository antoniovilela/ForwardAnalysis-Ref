#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/RecoAlgos/interface/TrackFullCloneSelectorBase.h"
#include "ForwardAnalysis/Utilities/interface/TrackWithJetSelector.h"

namespace reco { 
  namespace modules {

    typedef TrackFullCloneSelectorBase<forwardAnalysis::TrackWithJetSelector> TrackWithJetSelector;

    DEFINE_FWK_MODULE(TrackWithJetSelector);
  }
}
