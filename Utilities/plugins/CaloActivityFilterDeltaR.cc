#include "FWCore/Framework/interface/MakerMacros.h"
#include "ForwardAnalysis/Utilities/interface/CaloActivityFilter.h"
#include "ForwardAnalysis/Utilities/interface/DeltaRCaloTowerSelector.h"

typedef forwardAnalysis::CaloActivityFilter<forwardAnalysis::DeltaRCaloTowerSelector> CaloActivityFilterDeltaR;

DEFINE_FWK_MODULE(CaloActivityFilterDeltaR);
