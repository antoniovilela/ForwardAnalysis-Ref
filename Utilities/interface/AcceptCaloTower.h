#ifndef ForwardAnalysis_Utilities_AcceptCaloTower_h
#define ForwardAnalysis_Utilities_AcceptCaloTower_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class CaloTower;

namespace forwardAnalysis {

class AcceptCaloTower {
   public:
      AcceptCaloTower(edm::ParameterSet const& pset) {}
      ~AcceptCaloTower() {}

      bool operator() (CaloTower const& calotower, edm::Event const& event);
};

}

#endif
