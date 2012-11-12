#include "FWCore/Framework/interface/MakerMacros.h"

#include "CommonTools/RecoAlgos/interface/TrackFullCloneSelectorBase.h"
#include "Utilities/AnalysisTools/interface/TrackAnalysisSelector.h"

namespace reco { 
  namespace modules {

    typedef TrackFullCloneSelectorBase< analysisTools::TrackAnalysisSelector > TrackAnalysisSelector;

    //DEFINE_FWK_MODULE(TrackAnalysisSelector);
  }
}
