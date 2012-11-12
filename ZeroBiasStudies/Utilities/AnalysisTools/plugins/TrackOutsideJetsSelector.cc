#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/RecoAlgos/interface/TrackFullCloneSelectorBase.h"
#include "Utilities/AnalysisTools/interface/TrackOutsideJetsSelector.h"

namespace reco { 
  namespace modules {

    typedef TrackFullCloneSelectorBase< analysisTools::TrackOutsideJetsSelector > TrackOutsideJetsSelector;

    //DEFINE_FWK_MODULE(TrackOutsideJetsSelector);
  }
}
