#ifndef ForwardAnalysis_Utilities_CastorEnergy_h
#define ForwardAnalysis_Utilities_CastorEnergy_h

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include <vector>
#include <algorithm>

namespace forwardAnalysis {

class CastorEnergy {
  public:
     CastorEnergy(std::vector<int> const& modules): modules_(modules) {}
     ~CastorEnergy() {}

     double operator()(CastorRecHitCollection const&, bool isRealData = true);
  private:
     std::vector<int> modules_;
};
 
} // namespace
#endif
