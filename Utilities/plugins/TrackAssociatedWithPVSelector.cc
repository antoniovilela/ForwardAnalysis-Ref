#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/RecoAlgos/interface/TrackFullCloneSelectorBase.h"
#include "ForwardAnalysis/Utilities/interface/TrackAssociatedWithPVSelector.h"

namespace reco { 
  namespace modules {

    typedef TrackFullCloneSelectorBase<forwardAnalysis::TrackAssociatedWithPVSelector > TrackAssociatedWithPVSelector;

    DEFINE_FWK_MODULE(TrackAssociatedWithPVSelector);
  }
}
