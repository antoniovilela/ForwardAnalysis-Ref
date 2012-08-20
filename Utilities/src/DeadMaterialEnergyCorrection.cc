
#include "ForwardAnalysis/Utilities/interface/DeadMaterialEnergyCorrection.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include <vector>
#include <algorithm>

using forwardAnalysis::DeadMaterialEnergyCorrection;

double etaBinsHFBoundaries[] = { 2.866, 2.976,
                                 3.152, 3.327, 3.503, 3.677, 3.853, 4.027, 4.204, 4.377, 4.552,
                                 4.730, 4.903, 5.205}; // 13 bins

double DeadMaterialEnergyCorrection::operator()(CaloTower const& calotower){
 
  int ieta = calotower.ieta();
  double value = correction( ieta );
  return value; 
}

double DeadMaterialEnergyCorrection::operator()(reco::PFCandidate const& pfcandidate){
  std::vector<double> vec_etaBins(etaBinsHFBoundaries, etaBinsHFBoundaries + sizeof(etaBinsHFBoundaries)/sizeof(double));
  double eta = fabs( pfcandidate.eta() );
  std::vector<double>::const_iterator it_bin = std::upper_bound(vec_etaBins.begin(),vec_etaBins.end(),eta);
  int idx_vec = int(it_bin - vec_etaBins.begin());
  int ieta = 28 + idx_vec;
  double value = correction( ieta );
  return value; 
}

double DeadMaterialEnergyCorrection::correction(int ieta){

  double correction = 1.;
  if (abs(ieta) == 30)      correction = 0.982;
  else if (abs(ieta) == 31) correction = 0.978;
  else if (abs(ieta) == 32) correction = 0.974;
  else if (abs(ieta) == 33) correction = 0.969;
  else if (abs(ieta) == 34) correction = 0.963;
  else if (abs(ieta) == 35) correction = 0.956;
  else if (abs(ieta) == 36) correction = 0.948;
  else if (abs(ieta) == 37) correction = 0.938;
  else if (abs(ieta) == 38) correction = 0.926;
  else if (abs(ieta) == 39) correction = 0.912;
  else if (abs(ieta) == 40) correction = 0.895; 

  return correction;
}
