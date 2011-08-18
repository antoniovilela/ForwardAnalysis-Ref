#ifndef ForwardAnalysis_Utilities_AcceptCaloTowerSelector_h
#define ForwardAnalysis_Utilities_AcceptCaloTowerSelector_h

#include "ForwardAnalysis/Utilities/interface/ObjectToEventSelectorAdaptor.h"
#include "ForwardAnalysis/Utilities/interface/AcceptCaloTower.h"

class CaloTower;

namespace forwardAnalysis {

typedef ObjectToEventSelectorAdaptor<CaloTower,AcceptCaloTower> AcceptCaloTowerSelector;

}

#endif
