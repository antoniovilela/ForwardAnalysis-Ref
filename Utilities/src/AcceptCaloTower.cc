#include "ForwardAnalysis/Utilities/interface/AcceptCaloTower.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"

namespace forwardAnalysis {

bool AcceptCaloTower::operator() (CaloTower const& calotower) {
  int ieta = calotower.ieta();

  bool accept = true; 
  if( abs(ieta) == 29) accept = false;
  if( abs(ieta) == 30) accept = false;
  if( abs(ieta) >= 40) accept = false;

  //...

  return accept; 
}

}
