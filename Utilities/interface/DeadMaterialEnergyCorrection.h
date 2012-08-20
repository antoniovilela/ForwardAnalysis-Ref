#ifndef ForwardAnalysis_Utilities_DeadMaterialEnergyCorrection_h
#define ForwardAnalysis_Utilities_DeadMaterialEnergyCorrection_h

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

namespace forwardAnalysis {

class DeadMaterialEnergyCorrection {
  public:
     DeadMaterialEnergyCorrection() {}
     ~DeadMaterialEnergyCorrection() {}

     double operator()(CaloTower const&);
     double operator()(reco::PFCandidate const&);
  private:
     double correction(int);
};
 
} // namespace
#endif
