#include "FWCore/Framework/interface/MakerMacros.h"
#include "ForwardAnalysis/Utilities/interface/CaloActivityFilter.h"

typedef forwardAnalysis::CaloActivityFilter<> CaloActivityFilter;

DEFINE_FWK_MODULE(CaloActivityFilter);
