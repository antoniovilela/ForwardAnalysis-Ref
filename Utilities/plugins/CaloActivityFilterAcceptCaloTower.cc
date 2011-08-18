#include "FWCore/Framework/interface/MakerMacros.h"
#include "ForwardAnalysis/Utilities/interface/CaloActivityFilter.h"
#include "ForwardAnalysis/Utilities/interface/AcceptCaloTowerSelector.h"

typedef forwardAnalysis::CaloActivityFilter<forwardAnalysis::AcceptCaloTowerSelector> CaloActivityFilterAcceptCaloTower;

DEFINE_FWK_MODULE(CaloActivityFilterAcceptCaloTower);
