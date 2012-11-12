#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/RecoAlgos/interface/TrackFullCloneSelectorBase.h"
#include "Utilities/AnalysisTools/interface/TrackWithJetSelector.h"

namespace reco { 
  namespace modules {

    typedef TrackFullCloneSelectorBase< analysisTools::TrackWithJetSelector > TrackWithJetSelector;

    //DEFINE_FWK_MODULE(TrackWithJetSelector);
  }
}
