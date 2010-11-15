#ifndef ForwardAnalysis_Utilities_DeltaRCaloTowerSelector_h
#define ForwardAnalysis_Utilities_DeltaRCaloTowerSelector_h 

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CommonTools/UtilAlgos/interface/DeltaRMinPairSelector.h"
#include "ForwardAnalysis/Utilities/interface/OneToManySelector.h"

namespace forwardAnalysis {

typedef OneToManySelector<CaloTower,
                          edm::View<reco::Candidate>,
                          DeltaRMinPairSelector> DeltaRCaloTowerSelector;

} // namespace
#endif
