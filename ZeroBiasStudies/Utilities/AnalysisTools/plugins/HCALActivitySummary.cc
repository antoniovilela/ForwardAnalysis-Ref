
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "ForwardAnalysis/Utilities/interface/AcceptCaloTower.h"

#include <vector>

class CaloTower;

class HCALActivitySummary : public edm::EDProducer {
public:
  HCALActivitySummary( edm::ParameterSet const& );
private:
  void produce( edm::Event &, edm::EventSetup const& );
  bool checkTowerFlagsHB( CaloTower const& );
  bool checkTowerFlagsHE( CaloTower const& );
  bool checkTowerFlagsHF( CaloTower const&, double );

  forwardAnalysis::AcceptCaloTower acceptCaloTower_;
  edm::InputTag caloTowersTag_;

  double eThresholdHBMin_;
  double eThresholdHBMax_;
  double eThresholdHEMin_;
  double eThresholdHEMax_;
  double eThresholdHFMin_;
  double eThresholdHFMax_;
  double eThresholdEBMin_;
  double eThresholdEBMax_;
  double eThresholdEEMin_;
  double eThresholdEEMax_;
  unsigned int nThresholdIter_;
  bool discardFlaggedTowers_;
  bool applyEnergyScale_;
  double energyScaleHB_;
  double energyScaleHE_;
  double energyScaleHF_;
  double energyScaleEB_;
  double energyScaleEE_;
  std::vector<double> thresholdsHB_;
  std::vector<double> thresholdsHE_;
  std::vector<double> thresholdsHF_;
  std::vector<double> thresholdsEB_;
  std::vector<double> thresholdsEE_; 
};

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h" 
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

using namespace reco;

HCALActivitySummary::HCALActivitySummary(edm::ParameterSet const& pset):
  acceptCaloTower_(pset), 
  caloTowersTag_(pset.getParameter<edm::InputTag>("CaloTowersTag")),
  eThresholdHBMin_(pset.getParameter<double>("TowerEnergyThresholdHBMin")),
  eThresholdHBMax_(pset.getParameter<double>("TowerEnergyThresholdHBMax")),
  eThresholdHEMin_(pset.getParameter<double>("TowerEnergyThresholdHEMin")),
  eThresholdHEMax_(pset.getParameter<double>("TowerEnergyThresholdHEMax")),
  eThresholdHFMin_(pset.getParameter<double>("TowerEnergyThresholdHFMin")),
  eThresholdHFMax_(pset.getParameter<double>("TowerEnergyThresholdHFMax")),
  eThresholdEBMin_(pset.getParameter<double>("TowerEnergyThresholdEBMin")),
  eThresholdEBMax_(pset.getParameter<double>("TowerEnergyThresholdEBMax")),
  eThresholdEEMin_(pset.getParameter<double>("TowerEnergyThresholdEEMin")),
  eThresholdEEMax_(pset.getParameter<double>("TowerEnergyThresholdEEMax")), 
  nThresholdIter_(pset.getParameter<unsigned int>("NumberOfThresholds")),   
  discardFlaggedTowers_(pset.getParameter<bool>("DiscardFlaggedTowers")),
  applyEnergyScale_(pset.getParameter<bool>("ApplyEnergyScale")),
  energyScaleHB_(1.0),
  energyScaleHE_(1.0),
  energyScaleHF_(1.0),
  energyScaleEB_(1.0),
  energyScaleEE_(1.0) {

  if(applyEnergyScale_){
     energyScaleHB_ = pset.getParameter<double>("EnergyScaleFactorHB");
     energyScaleHE_ = pset.getParameter<double>("EnergyScaleFactorHE");
     energyScaleHF_ = pset.getParameter<double>("EnergyScaleFactorHF");
     energyScaleEB_ = pset.getParameter<double>("EnergyScaleFactorEB");
     energyScaleEE_ = pset.getParameter<double>("EnergyScaleFactorEE");
  }

  thresholdsHB_ = std::vector<double>(nThresholdIter_);
  thresholdsHE_ = std::vector<double>(nThresholdIter_);
  thresholdsHF_ = std::vector<double>(nThresholdIter_);
  thresholdsEB_ = std::vector<double>(nThresholdIter_);
  thresholdsEE_ = std::vector<double>(nThresholdIter_); 
  for(size_t k = 0; k < nThresholdIter_; ++k){
     thresholdsHB_[k] = eThresholdHBMin_ + k*((eThresholdHBMax_ - eThresholdHBMin_)/nThresholdIter_);
     thresholdsHE_[k] = eThresholdHEMin_ + k*((eThresholdHEMax_ - eThresholdHEMin_)/nThresholdIter_);
     thresholdsHF_[k] = eThresholdHFMin_ + k*((eThresholdHFMax_ - eThresholdHFMin_)/nThresholdIter_);
     thresholdsEB_[k] = eThresholdEBMin_ + k*((eThresholdEBMax_ - eThresholdEBMin_)/nThresholdIter_);
     thresholdsEE_[k] = eThresholdEEMin_ + k*((eThresholdEEMax_ - eThresholdEEMin_)/nThresholdIter_);
  }
    
  std::string alias;
  // HB
  //FIXME: thresholds should not be event by event product
  produces<std::vector<double> >( alias = "thresholdsHB" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHBplus" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHBminus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHBplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHBminus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumETHBplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumETHBminus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHBMultiplicityPlus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHBMultiplicityMinus" ).setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHBEnergySumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHBEnergySumMinus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHBETSumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHBETSumMinus").setBranchAlias( alias );
  // HE
  produces<std::vector<double> >( alias = "thresholdsHE" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHEplus" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHEminus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHEplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHEminus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumETHEplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumETHEminus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHEMultiplicityPlus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHEMultiplicityMinus" ).setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHEEnergySumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHEEnergySumMinus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHEETSumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHEETSumMinus").setBranchAlias( alias );
  // HF
  produces<std::vector<double> >( alias = "thresholdsHF" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHFplus" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nHFminus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHFplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEHFminus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumETHFplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumETHFminus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHFMultiplicityPlus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHFMultiplicityMinus" ).setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHFEnergySumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHFEnergySumMinus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHFETSumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHFETSumMinus").setBranchAlias( alias );
  // EB
  produces<std::vector<double> >( alias = "thresholdsEB" ).setBranchAlias( alias );  produces<std::vector<unsigned int> >( alias = "nEBplus" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nEBminus" ).setBranchAlias( alias );  produces<std::vector<double> >( alias = "sumEEBplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEEBminus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumETEBplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumETEBminus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaEBMultiplicityPlus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaEBMultiplicityMinus" ).setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaEBEnergySumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaEBEnergySumMinus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaEBETSumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaEBETSumMinus").setBranchAlias( alias );
  // EE
  produces<std::vector<double> >( alias = "thresholdsEE" ).setBranchAlias( alias );  produces<std::vector<unsigned int> >( alias = "nEEplus" ).setBranchAlias( alias );
  produces<std::vector<unsigned int> >( alias = "nEEminus" ).setBranchAlias( alias );  produces<std::vector<double> >( alias = "sumEEEplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumEEEminus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumETEEplus" ).setBranchAlias( alias );
  produces<std::vector<double> >( alias = "sumETEEminus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaEEMultiplicityPlus" ).setBranchAlias( alias );
  produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaEEMultiplicityMinus" ).setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaEEEnergySumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaEEEnergySumMinus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaEEETSumPlus").setBranchAlias( alias );
  produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaEEETSumMinus").setBranchAlias( alias );
}

void HCALActivitySummary::produce(edm::Event& event, edm::EventSetup const& setup) {

  edm::Handle<CaloTowerCollection> towerCollectionH;
  event.getByLabel(caloTowersTag_,towerCollectionH);
  const CaloTowerCollection& towerCollection = *(towerCollectionH.product());
	
  // Loop over CaloTowers
  std::map<int,std::vector<unsigned int> > iEtaHBMultiplicity;
  std::map<int,std::vector<double> > iEtaHBEnergySum;
  std::map<int,std::vector<double> > iEtaHBETSum;
  std::map<int,std::vector<unsigned int> > iEtaHEMultiplicity;
  std::map<int,std::vector<double> > iEtaHEEnergySum;
  std::map<int,std::vector<double> > iEtaHEETSum;
  std::map<int,std::vector<unsigned int> > iEtaHFMultiplicity;
  std::map<int,std::vector<double> > iEtaHFEnergySum;
  std::map<int,std::vector<double> > iEtaHFETSum;
  std::map<int,std::vector<unsigned int> > iEtaEBMultiplicity;
  std::map<int,std::vector<double> > iEtaEBEnergySum;
  std::map<int,std::vector<double> > iEtaEBETSum;
  std::map<int,std::vector<unsigned int> > iEtaEEMultiplicity;
  std::map<int,std::vector<double> > iEtaEEEnergySum;
  std::map<int,std::vector<double> > iEtaEEETSum;

  CaloTowerCollection::const_iterator calotower = towerCollection.begin();
  CaloTowerCollection::const_iterator calotowers_end = towerCollection.end(); 
  for(; calotower != calotowers_end; ++calotower) {	
     
     // General calo tower selector
     if( !acceptCaloTower_(*calotower) ) continue;
 
     bool hasHCAL = false;		
     bool hasHF = false;
     bool hasHE = false;
     bool hasHB = false;
     bool hasHO = false;
     bool hasECAL = false; 
     bool hasEB = false;
     bool hasEE = false;

     for(size_t iconst = 0; iconst < calotower->constituentsSize(); iconst++){
        DetId detId = calotower->constituent(iconst);
        if( detId.det() == DetId::Hcal ){
           hasHCAL = true;
           HcalDetId hcalDetId(detId);
           if( hcalDetId.subdet() == HcalForward ) hasHF = true;
           else if( hcalDetId.subdet() == HcalEndcap ) hasHE = true;
           else if( hcalDetId.subdet() == HcalBarrel ) hasHB = true;
           else if( hcalDetId.subdet() == HcalOuter ) hasHO = true;
        } else if( detId.det() == DetId::Ecal ){
           hasECAL = true;
           EcalSubdetector ecalSubDet = (EcalSubdetector)detId.subdetId();
           if(ecalSubDet == EcalBarrel) hasEB = true;
           else if(ecalSubDet == EcalEndcap) hasEE = true;
        }
     }
	
     //int zside = calotower->zside();
     //double eta = calotower->eta();
     int ieta = calotower->ieta();
     double energy = calotower->energy();
     double emEnergy = calotower->emEnergy();
     double hadEnergy = calotower->hadEnergy();

     // FIXME: Determine ET from vertex position
     //double towerEt = calotower->et(primVtx.position());
     //double towerEt = calotower->et(primVtx.z());
     double towerEt = calotower->et(); 
     double towerEmEt = calotower->emEt();
     double towerHadEt = calotower->hadEt();  

     // HCAL
     bool isHBTower = hasHB && (!hasHE);
     bool isHETower = hasHE && (!hasHB) && (!hasHF);
     bool isHFTower = hasHF && (!hasHE);
     // ECAL
     bool isEBTower = hasEB && (!hasEE);
     bool isEETower = hasEE && (!hasEB);
     
     // HCAL 
     if(isHBTower){
        bool acceptHBTower = true;
        if(discardFlaggedTowers_ && checkTowerFlagsHB(*calotower)) acceptHBTower = false; 
        if(!acceptHBTower) continue;

        if(applyEnergyScale_) { hadEnergy *= energyScaleHB_; towerHadEt *= energyScaleHB_; }
     } else if(isHETower){
        bool acceptHETower = true;
        if(discardFlaggedTowers_ && checkTowerFlagsHE(*calotower)) acceptHETower = false;     
        if(!acceptHETower) continue;

        if(applyEnergyScale_) { hadEnergy *= energyScaleHE_; towerHadEt *= energyScaleHE_; }
     } else if(isHFTower){
        bool acceptHFTower = true; 
        if(discardFlaggedTowers_ && checkTowerFlagsHF(*calotower,0.99)) acceptHFTower = false;
        if(!acceptHFTower) continue;

        if(applyEnergyScale_) {
           energy *= energyScaleHF_; towerEt *= energyScaleHF_;
           emEnergy *= energyScaleHF_; towerEmEt *= energyScaleHF_;
           hadEnergy *= energyScaleHF_; towerHadEt *= energyScaleHF_;
        } 
     }
     // ECAL
     if(isEBTower){
        if(applyEnergyScale_) { emEnergy *= energyScaleEB_; towerEmEt *= energyScaleEB_; } 
     } else if(isEETower){
        if(applyEnergyScale_) { emEnergy *= energyScaleEE_; towerEmEt *= energyScaleEE_; }
     }
 
     // Loop over tower thresholds
     // For HB/HE use had. energy; for HF energy; for EB/EE em. energy
     for(size_t i_threshold = 0; i_threshold < nThresholdIter_; ++i_threshold){ 
        // HCAL
        if( isHBTower && (hadEnergy >= thresholdsHB_[i_threshold]) ){
           if(iEtaHBMultiplicity.find(ieta) == iEtaHBMultiplicity.end()){
              iEtaHBMultiplicity[ieta] = std::vector<unsigned int>(nThresholdIter_);
              iEtaHBEnergySum[ieta] = std::vector<double>(nThresholdIter_);
              iEtaHBETSum[ieta] = std::vector<double>(nThresholdIter_);
           }
           ++iEtaHBMultiplicity[ieta][i_threshold];
           iEtaHBEnergySum[ieta][i_threshold] += hadEnergy;
           iEtaHBETSum[ieta][i_threshold] += towerHadEt;
        } else if( isHETower && (hadEnergy >= thresholdsHE_[i_threshold]) ){
           if(iEtaHEMultiplicity.find(ieta) == iEtaHEMultiplicity.end()){
              iEtaHEMultiplicity[ieta] = std::vector<unsigned int>(nThresholdIter_);
              iEtaHEEnergySum[ieta] = std::vector<double>(nThresholdIter_);
              iEtaHEETSum[ieta] = std::vector<double>(nThresholdIter_);
           }
           ++iEtaHEMultiplicity[ieta][i_threshold];
           iEtaHEEnergySum[ieta][i_threshold] += hadEnergy;
           iEtaHEETSum[ieta][i_threshold] += towerHadEt;
        } else if( isHFTower && (energy >= thresholdsHF_[i_threshold]) ){
           if(iEtaHFMultiplicity.find(ieta) == iEtaHFMultiplicity.end()){
              iEtaHFMultiplicity[ieta] = std::vector<unsigned int>(nThresholdIter_);
              iEtaHFEnergySum[ieta] = std::vector<double>(nThresholdIter_);
              iEtaHFETSum[ieta] = std::vector<double>(nThresholdIter_);
           }
           ++iEtaHFMultiplicity[ieta][i_threshold];
           iEtaHFEnergySum[ieta][i_threshold] += energy;
           iEtaHFETSum[ieta][i_threshold] += towerEt;
        }
        // ECAL
        if( isEBTower && (emEnergy >= thresholdsEB_[i_threshold]) ){
           if(iEtaEBMultiplicity.find(ieta) == iEtaEBMultiplicity.end()){
              iEtaEBMultiplicity[ieta] = std::vector<unsigned int>(nThresholdIter_);
              iEtaEBEnergySum[ieta] = std::vector<double>(nThresholdIter_);
              iEtaEBETSum[ieta] = std::vector<double>(nThresholdIter_);
           }
           ++iEtaEBMultiplicity[ieta][i_threshold];
           iEtaEBEnergySum[ieta][i_threshold] += emEnergy;
           iEtaEBETSum[ieta][i_threshold] += towerEmEt;
        } else if( isEETower && (emEnergy >= thresholdsEE_[i_threshold]) ){
           if(iEtaEEMultiplicity.find(ieta) == iEtaEEMultiplicity.end()){
              iEtaEEMultiplicity[ieta] = std::vector<unsigned int>(nThresholdIter_);
              iEtaEEEnergySum[ieta] = std::vector<double>(nThresholdIter_);
              iEtaEEETSum[ieta] = std::vector<double>(nThresholdIter_);
           }
           ++iEtaEEMultiplicity[ieta][i_threshold];
           iEtaEEEnergySum[ieta][i_threshold] += emEnergy;
           iEtaEEETSum[ieta][i_threshold] += towerEmEt;
        }
     }
  }

  // HB
  std::vector<unsigned int> nHB_plus(nThresholdIter_,0);
  std::vector<unsigned int> nHB_minus(nThresholdIter_,0);
  std::vector<double> sumEHB_plus(nThresholdIter_,0.);
  std::vector<double> sumEHB_minus(nThresholdIter_,0.);
  std::vector<double> sumETHB_plus(nThresholdIter_,0.);
  std::vector<double> sumETHB_minus(nThresholdIter_,0.);
  std::map<unsigned int,std::vector<unsigned int> > iEtaHBMultiplicity_plus;
  std::map<unsigned int,std::vector<unsigned int> > iEtaHBMultiplicity_minus;
  std::map<unsigned int,std::vector<double> > iEtaHBEnergySum_plus;
  std::map<unsigned int,std::vector<double> > iEtaHBEnergySum_minus;
  std::map<unsigned int,std::vector<double> > iEtaHBETSum_plus;
  std::map<unsigned int,std::vector<double> > iEtaHBETSum_minus; 

  std::map<int,std::vector<unsigned int> >::const_iterator itHBMult = iEtaHBMultiplicity.begin();
  std::map<int,std::vector<unsigned int> >::const_iterator itHBMult_end = iEtaHBMultiplicity.end();
  for(; itHBMult != itHBMult_end; ++itHBMult){
     int ieta = itHBMult->first;
     std::vector<unsigned int> const& vec_mult = iEtaHBMultiplicity[ieta];
     std::vector<double> const& vec_esum = iEtaHBEnergySum[ieta];  
     std::vector<double> const& vec_etsum = iEtaHBETSum[ieta];
     for(size_t i_threshold = 0; i_threshold < nThresholdIter_; ++i_threshold){
        if(ieta >= 0){
           nHB_plus[i_threshold] += vec_mult[i_threshold];
           sumEHB_plus[i_threshold] += vec_esum[i_threshold];
           sumETHB_plus[i_threshold] += vec_etsum[i_threshold];
        } else{
           nHB_minus[i_threshold] += vec_mult[i_threshold];
           sumEHB_minus[i_threshold] += vec_esum[i_threshold];
           sumETHB_minus[i_threshold] += vec_etsum[i_threshold];
        } 
     }

     unsigned int abs_ieta = (ieta >= 0) ? ieta : -ieta;
     if(ieta >= 0){
        iEtaHBMultiplicity_plus[abs_ieta] = vec_mult;
        iEtaHBEnergySum_plus[abs_ieta] = vec_esum;
        iEtaHBETSum_plus[abs_ieta] = vec_etsum;
     } else{
        iEtaHBMultiplicity_minus[abs_ieta] = vec_mult;
        iEtaHBEnergySum_minus[abs_ieta] = vec_esum;
        iEtaHBETSum_minus[abs_ieta] = vec_etsum;
     }
  }

  // HE
  std::vector<unsigned int> nHE_plus(nThresholdIter_,0);
  std::vector<unsigned int> nHE_minus(nThresholdIter_,0);
  std::vector<double> sumEHE_plus(nThresholdIter_,0.);
  std::vector<double> sumEHE_minus(nThresholdIter_,0.);
  std::vector<double> sumETHE_plus(nThresholdIter_,0.);
  std::vector<double> sumETHE_minus(nThresholdIter_,0.);
  std::map<unsigned int,std::vector<unsigned int> > iEtaHEMultiplicity_plus;
  std::map<unsigned int,std::vector<unsigned int> > iEtaHEMultiplicity_minus;
  std::map<unsigned int,std::vector<double> > iEtaHEEnergySum_plus;
  std::map<unsigned int,std::vector<double> > iEtaHEEnergySum_minus;
  std::map<unsigned int,std::vector<double> > iEtaHEETSum_plus;
  std::map<unsigned int,std::vector<double> > iEtaHEETSum_minus; 

  std::map<int,std::vector<unsigned int> >::const_iterator itHEMult = iEtaHEMultiplicity.begin();
  std::map<int,std::vector<unsigned int> >::const_iterator itHEMult_end = iEtaHEMultiplicity.end();
  for(; itHEMult != itHEMult_end; ++itHEMult){
     int ieta = itHEMult->first;
     std::vector<unsigned int> const& vec_mult = iEtaHEMultiplicity[ieta];
     std::vector<double> const& vec_esum = iEtaHEEnergySum[ieta];
     std::vector<double> const& vec_etsum = iEtaHEETSum[ieta];
     for(size_t i_threshold = 0; i_threshold < nThresholdIter_; ++i_threshold){
        if(ieta >= 0){
           nHE_plus[i_threshold] += vec_mult[i_threshold];
           sumEHE_plus[i_threshold] += vec_esum[i_threshold];
           sumETHE_plus[i_threshold] += vec_etsum[i_threshold];
        } else{
           nHE_minus[i_threshold] += vec_mult[i_threshold];
           sumEHE_minus[i_threshold] += vec_esum[i_threshold];
           sumETHE_minus[i_threshold] += vec_etsum[i_threshold];
        }
     } 

     unsigned int abs_ieta = (ieta >= 0) ? ieta : -ieta;
     if(ieta >= 0){
        iEtaHEMultiplicity_plus[abs_ieta] = vec_mult;
        iEtaHEEnergySum_plus[abs_ieta] = vec_esum;
        iEtaHEETSum_plus[abs_ieta] = vec_etsum;
     } else{
        iEtaHEMultiplicity_minus[abs_ieta] = vec_mult;
        iEtaHEEnergySum_minus[abs_ieta] = vec_esum;
        iEtaHEETSum_minus[abs_ieta] = vec_etsum;
     }
  }  

  // HF
  std::vector<unsigned int> nHF_plus(nThresholdIter_,0);
  std::vector<unsigned int> nHF_minus(nThresholdIter_,0);
  std::vector<double> sumEHF_plus(nThresholdIter_,0.);
  std::vector<double> sumEHF_minus(nThresholdIter_,0.);
  std::vector<double> sumETHF_plus(nThresholdIter_,0.);
  std::vector<double> sumETHF_minus(nThresholdIter_,0.);
  std::map<unsigned int,std::vector<unsigned int> > iEtaHFMultiplicity_plus;
  std::map<unsigned int,std::vector<unsigned int> > iEtaHFMultiplicity_minus;
  std::map<unsigned int,std::vector<double> > iEtaHFEnergySum_plus;
  std::map<unsigned int,std::vector<double> > iEtaHFEnergySum_minus;
  std::map<unsigned int,std::vector<double> > iEtaHFETSum_plus;
  std::map<unsigned int,std::vector<double> > iEtaHFETSum_minus;

  std::map<int,std::vector<unsigned int> >::const_iterator itHFMult = iEtaHFMultiplicity.begin();
  std::map<int,std::vector<unsigned int> >::const_iterator itHFMult_end = iEtaHFMultiplicity.end();
  for(; itHFMult != itHFMult_end; ++itHFMult){
     int ieta = itHFMult->first;
     std::vector<unsigned int> const& vec_mult = iEtaHFMultiplicity[ieta];
     std::vector<double> const& vec_esum = iEtaHFEnergySum[ieta];
     std::vector<double> const& vec_etsum = iEtaHFETSum[ieta];
     for(size_t i_threshold = 0; i_threshold < nThresholdIter_; ++i_threshold){
        if(ieta >= 0){
           nHF_plus[i_threshold] += vec_mult[i_threshold];
           sumEHF_plus[i_threshold] += vec_esum[i_threshold];
           sumETHF_plus[i_threshold] += vec_etsum[i_threshold];
        } else{
           nHF_minus[i_threshold] += vec_mult[i_threshold];
           sumEHF_minus[i_threshold] += vec_esum[i_threshold];
           sumETHF_minus[i_threshold] += vec_etsum[i_threshold];
        }
     }

     unsigned int abs_ieta = (ieta >= 0) ? ieta : -ieta;
     if(ieta >= 0){
        iEtaHFMultiplicity_plus[abs_ieta] = vec_mult;
        iEtaHFEnergySum_plus[abs_ieta] = vec_esum;
        iEtaHFETSum_plus[abs_ieta] = vec_etsum;
     } else{
        iEtaHFMultiplicity_minus[abs_ieta] = vec_mult;
        iEtaHFEnergySum_minus[abs_ieta] = vec_esum;
        iEtaHFETSum_minus[abs_ieta] = vec_etsum;
     } 
  }

  // EB
  std::vector<unsigned int> nEB_plus(nThresholdIter_,0);
  std::vector<unsigned int> nEB_minus(nThresholdIter_,0);
  std::vector<double> sumEEB_plus(nThresholdIter_,0.);
  std::vector<double> sumEEB_minus(nThresholdIter_,0.);
  std::vector<double> sumETEB_plus(nThresholdIter_,0.);
  std::vector<double> sumETEB_minus(nThresholdIter_,0.);
  std::map<unsigned int,std::vector<unsigned int> > iEtaEBMultiplicity_plus;
  std::map<unsigned int,std::vector<unsigned int> > iEtaEBMultiplicity_minus;
  std::map<unsigned int,std::vector<double> > iEtaEBEnergySum_plus;
  std::map<unsigned int,std::vector<double> > iEtaEBEnergySum_minus;
  std::map<unsigned int,std::vector<double> > iEtaEBETSum_plus;
  std::map<unsigned int,std::vector<double> > iEtaEBETSum_minus;

  std::map<int,std::vector<unsigned int> >::const_iterator itEBMult = iEtaEBMultiplicity.begin();
  std::map<int,std::vector<unsigned int> >::const_iterator itEBMult_end = iEtaEBMultiplicity.end();
  for(; itEBMult != itEBMult_end; ++itEBMult){
     int ieta = itEBMult->first;
     std::vector<unsigned int> const& vec_mult = iEtaEBMultiplicity[ieta];
     std::vector<double> const& vec_esum = iEtaEBEnergySum[ieta];
     std::vector<double> const& vec_etsum = iEtaEBETSum[ieta];
     for(size_t i_threshold = 0; i_threshold < nThresholdIter_; ++i_threshold){
        if(ieta >= 0){
           nEB_plus[i_threshold] += vec_mult[i_threshold];
           sumEEB_plus[i_threshold] += vec_esum[i_threshold];
           sumETEB_plus[i_threshold] += vec_etsum[i_threshold];
        } else{
           nEB_minus[i_threshold] += vec_mult[i_threshold];
           sumEEB_minus[i_threshold] += vec_esum[i_threshold];
           sumETEB_minus[i_threshold] += vec_etsum[i_threshold];
        }
     }

     unsigned int abs_ieta = (ieta >= 0) ? ieta : -ieta;
     if(ieta >= 0){
        iEtaEBMultiplicity_plus[abs_ieta] = vec_mult;
        iEtaEBEnergySum_plus[abs_ieta] = vec_esum;
        iEtaEBETSum_plus[abs_ieta] = vec_etsum;
     } else{
        iEtaEBMultiplicity_minus[abs_ieta] = vec_mult;
        iEtaEBEnergySum_minus[abs_ieta] = vec_esum;
        iEtaEBETSum_minus[abs_ieta] = vec_etsum;
     }
  }
  
  // EE
  std::vector<unsigned int> nEE_plus(nThresholdIter_,0);
  std::vector<unsigned int> nEE_minus(nThresholdIter_,0);
  std::vector<double> sumEEE_plus(nThresholdIter_,0.);
  std::vector<double> sumEEE_minus(nThresholdIter_,0.);
  std::vector<double> sumETEE_plus(nThresholdIter_,0.);
  std::vector<double> sumETEE_minus(nThresholdIter_,0.);
  std::map<unsigned int,std::vector<unsigned int> > iEtaEEMultiplicity_plus;
  std::map<unsigned int,std::vector<unsigned int> > iEtaEEMultiplicity_minus;
  std::map<unsigned int,std::vector<double> > iEtaEEEnergySum_plus;
  std::map<unsigned int,std::vector<double> > iEtaEEEnergySum_minus;
  std::map<unsigned int,std::vector<double> > iEtaEEETSum_plus;
  std::map<unsigned int,std::vector<double> > iEtaEEETSum_minus;
  
  std::map<int,std::vector<unsigned int> >::const_iterator itEEMult = iEtaEEMultiplicity.begin();
  std::map<int,std::vector<unsigned int> >::const_iterator itEEMult_end = iEtaEEMultiplicity.end();
  for(; itEEMult != itEEMult_end; ++itEEMult){
     int ieta = itEEMult->first;
     std::vector<unsigned int> const& vec_mult = iEtaEEMultiplicity[ieta];
     std::vector<double> const& vec_esum = iEtaEEEnergySum[ieta];
     std::vector<double> const& vec_etsum = iEtaEEETSum[ieta];
     for(size_t i_threshold = 0; i_threshold < nThresholdIter_; ++i_threshold){
        if(ieta >= 0){
           nEE_plus[i_threshold] += vec_mult[i_threshold];
           sumEEE_plus[i_threshold] += vec_esum[i_threshold];
           sumETEE_plus[i_threshold] += vec_etsum[i_threshold];
        } else{
           nEE_minus[i_threshold] += vec_mult[i_threshold];
           sumEEE_minus[i_threshold] += vec_esum[i_threshold];
           sumETEE_minus[i_threshold] += vec_etsum[i_threshold];
        }
     }

     unsigned int abs_ieta = (ieta >= 0) ? ieta : -ieta;
     if(ieta >= 0){
        iEtaEEMultiplicity_plus[abs_ieta] = vec_mult;
        iEtaEEEnergySum_plus[abs_ieta] = vec_esum;
        iEtaEEETSum_plus[abs_ieta] = vec_etsum;
     } else{
        iEtaEEMultiplicity_minus[abs_ieta] = vec_mult;
        iEtaEEEnergySum_minus[abs_ieta] = vec_esum;
        iEtaEEETSum_minus[abs_ieta] = vec_etsum;
     }
  }

  // HB
  std::auto_ptr<std::vector<double> > thresholdsHB_ptr( new std::vector<double>(thresholdsHB_) );
  std::auto_ptr<std::vector<unsigned int> > nHBplus_ptr( new std::vector<unsigned int>(nHB_plus) );
  std::auto_ptr<std::vector<unsigned int> > nHBminus_ptr( new std::vector<unsigned int>(nHB_minus) );
  std::auto_ptr<std::vector<double> > sumEHBplus_ptr( new std::vector<double>(sumEHB_plus) );
  std::auto_ptr<std::vector<double> > sumEHBminus_ptr( new std::vector<double>(sumEHB_minus) );
  std::auto_ptr<std::vector<double> > sumETHBplus_ptr( new std::vector<double>(sumETHB_plus) );
  std::auto_ptr<std::vector<double> > sumETHBminus_ptr( new std::vector<double>(sumETHB_minus) ); 
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHBMultiplicityPlus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHBMultiplicity_plus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHBMultiplicityMinus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHBMultiplicity_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHBEnergySumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHBEnergySum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHBEnergySumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHBEnergySum_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHBETSumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHBETSum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHBETSumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHBETSum_minus) );
  // HE  
  std::auto_ptr<std::vector<double> > thresholdsHE_ptr( new std::vector<double>(thresholdsHE_) );
  std::auto_ptr<std::vector<unsigned int> > nHEplus_ptr( new std::vector<unsigned int>(nHE_plus) );
  std::auto_ptr<std::vector<unsigned int> > nHEminus_ptr( new std::vector<unsigned int>(nHE_minus) );
  std::auto_ptr<std::vector<double> > sumEHEplus_ptr( new std::vector<double>(sumEHE_plus) );
  std::auto_ptr<std::vector<double> > sumEHEminus_ptr( new std::vector<double>(sumEHE_minus) );
  std::auto_ptr<std::vector<double> > sumETHEplus_ptr( new std::vector<double>(sumETHE_plus) );
  std::auto_ptr<std::vector<double> > sumETHEminus_ptr( new std::vector<double>(sumETHE_minus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHEMultiplicityPlus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHEMultiplicity_plus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHEMultiplicityMinus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHEMultiplicity_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHEEnergySumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHEEnergySum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHEEnergySumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHEEnergySum_minus) );  
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHEETSumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHEETSum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHEETSumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHEETSum_minus) );
  // HF
  std::auto_ptr<std::vector<double> > thresholdsHF_ptr( new std::vector<double>(thresholdsHF_) );
  std::auto_ptr<std::vector<unsigned int> > nHFplus_ptr( new std::vector<unsigned int>(nHF_plus) );
  std::auto_ptr<std::vector<unsigned int> > nHFminus_ptr( new std::vector<unsigned int>(nHF_minus) );
  std::auto_ptr<std::vector<double> > sumEHFplus_ptr( new std::vector<double>(sumEHF_plus) );
  std::auto_ptr<std::vector<double> > sumEHFminus_ptr( new std::vector<double>(sumEHF_minus) );
  std::auto_ptr<std::vector<double> > sumETHFplus_ptr( new std::vector<double>(sumETHF_plus) );
  std::auto_ptr<std::vector<double> > sumETHFminus_ptr( new std::vector<double>(sumETHF_minus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityPlus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHFMultiplicity_plus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityMinus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaHFMultiplicity_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHFEnergySumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHFEnergySum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHFEnergySumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHFEnergySum_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHFETSumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHFETSum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHFETSumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaHFETSum_minus) ); 
  // EB
  std::auto_ptr<std::vector<double> > thresholdsEB_ptr( new std::vector<double>(thresholdsEB_) );
  std::auto_ptr<std::vector<unsigned int> > nEBplus_ptr( new std::vector<unsigned int>(nEB_plus) );
  std::auto_ptr<std::vector<unsigned int> > nEBminus_ptr( new std::vector<unsigned int>(nEB_minus) );
  std::auto_ptr<std::vector<double> > sumEEBplus_ptr( new std::vector<double>(sumEEB_plus) );
  std::auto_ptr<std::vector<double> > sumEEBminus_ptr( new std::vector<double>(sumEEB_minus) );
  std::auto_ptr<std::vector<double> > sumETEBplus_ptr( new std::vector<double>(sumETEB_plus) );
  std::auto_ptr<std::vector<double> > sumETEBminus_ptr( new std::vector<double>(sumETEB_minus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaEBMultiplicityPlus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaEBMultiplicity_plus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaEBMultiplicityMinus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaEBMultiplicity_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaEBEnergySumPlus_ptr( new std::map<unsigned int, std::vector<double> >(iEtaEBEnergySum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaEBEnergySumMinus_ptr( new std::map<unsigned int, std::vector<double> >(iEtaEBEnergySum_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaEBETSumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaEBETSum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaEBETSumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaEBETSum_minus) );
  // EE
  std::auto_ptr<std::vector<double> > thresholdsEE_ptr( new std::vector<double>(thresholdsEE_) );
  std::auto_ptr<std::vector<unsigned int> > nEEplus_ptr( new std::vector<unsigned int>(nEE_plus) );
  std::auto_ptr<std::vector<unsigned int> > nEEminus_ptr( new std::vector<unsigned int>(nEE_minus) );
  std::auto_ptr<std::vector<double> > sumEEEplus_ptr( new std::vector<double>(sumEEE_plus) );
  std::auto_ptr<std::vector<double> > sumEEEminus_ptr( new std::vector<double>(sumEEE_minus) );
  std::auto_ptr<std::vector<double> > sumETEEplus_ptr( new std::vector<double>(sumETEE_plus) );
  std::auto_ptr<std::vector<double> > sumETEEminus_ptr( new std::vector<double>(sumETEE_minus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaEEMultiplicityPlus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaEEMultiplicity_plus) );
  std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaEEMultiplicityMinus_ptr( new std::map<unsigned int, std::vector<unsigned int> >(iEtaEEMultiplicity_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaEEEnergySumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaEEEnergySum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaEEEnergySumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaEEEnergySum_minus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaEEETSumPlus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaEEETSum_plus) );
  std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaEEETSumMinus_ptr( new std::map<unsigned int,std::vector<double> >(iEtaEEETSum_minus) );
  
  event.put( thresholdsHB_ptr, "thresholdsHB" );
  event.put( nHBplus_ptr, "nHBplus" );
  event.put( nHBminus_ptr, "nHBminus" );
  event.put( sumEHBplus_ptr, "sumEHBplus" );
  event.put( sumEHBminus_ptr, "sumEHBminus" );
  event.put( sumETHBplus_ptr, "sumETHBplus" );
  event.put( sumETHBminus_ptr, "sumETHBminus" );
  event.put( iEtaHBMultiplicityPlus_ptr, "iEtaHBMultiplicityPlus" );
  event.put( iEtaHBMultiplicityMinus_ptr, "iEtaHBMultiplicityMinus" );
  event.put( iEtaHBEnergySumPlus_ptr, "iEtaHBEnergySumPlus" );
  event.put( iEtaHBEnergySumMinus_ptr, "iEtaHBEnergySumMinus" );
  event.put( iEtaHBETSumPlus_ptr, "iEtaHBETSumPlus" );
  event.put( iEtaHBETSumMinus_ptr, "iEtaHBETSumMinus" );

  event.put( thresholdsHE_ptr, "thresholdsHE" );
  event.put( nHEplus_ptr, "nHEplus" );
  event.put( nHEminus_ptr, "nHEminus" );
  event.put( sumEHEplus_ptr, "sumEHEplus" );
  event.put( sumEHEminus_ptr, "sumEHEminus" );
  event.put( sumETHEplus_ptr, "sumETHEplus" );
  event.put( sumETHEminus_ptr, "sumETHEminus" );
  event.put( iEtaHEMultiplicityPlus_ptr, "iEtaHEMultiplicityPlus" );
  event.put( iEtaHEMultiplicityMinus_ptr, "iEtaHEMultiplicityMinus" );
  event.put( iEtaHEEnergySumPlus_ptr, "iEtaHEEnergySumPlus" );
  event.put( iEtaHEEnergySumMinus_ptr, "iEtaHEEnergySumMinus" );
  event.put( iEtaHEETSumPlus_ptr, "iEtaHEETSumPlus" );
  event.put( iEtaHEETSumMinus_ptr, "iEtaHEETSumMinus" );

  event.put( thresholdsHF_ptr, "thresholdsHF" );
  event.put( nHFplus_ptr, "nHFplus" );
  event.put( nHFminus_ptr, "nHFminus" );
  event.put( sumEHFplus_ptr, "sumEHFplus" );
  event.put( sumEHFminus_ptr, "sumEHFminus" );
  event.put( sumETHFplus_ptr, "sumETHFplus" );
  event.put( sumETHFminus_ptr, "sumETHFminus" );
  event.put( iEtaHFMultiplicityPlus_ptr, "iEtaHFMultiplicityPlus" );
  event.put( iEtaHFMultiplicityMinus_ptr, "iEtaHFMultiplicityMinus" );
  event.put( iEtaHFEnergySumPlus_ptr, "iEtaHFEnergySumPlus" );
  event.put( iEtaHFEnergySumMinus_ptr, "iEtaHFEnergySumMinus" );
  event.put( iEtaHFETSumPlus_ptr, "iEtaHFETSumPlus" );
  event.put( iEtaHFETSumMinus_ptr, "iEtaHFETSumMinus" );

  event.put( thresholdsEB_ptr, "thresholdsEB" );
  event.put( nEBplus_ptr, "nEBplus" );
  event.put( nEBminus_ptr, "nEBminus" );
  event.put( sumEEBplus_ptr, "sumEEBplus" );
  event.put( sumEEBminus_ptr, "sumEEBminus" );
  event.put( sumETEBplus_ptr, "sumETEBplus" );
  event.put( sumETEBminus_ptr, "sumETEBminus" );
  event.put( iEtaEBMultiplicityPlus_ptr, "iEtaEBMultiplicityPlus" );
  event.put( iEtaEBMultiplicityMinus_ptr, "iEtaEBMultiplicityMinus" );
  event.put( iEtaEBEnergySumPlus_ptr, "iEtaEBEnergySumPlus" );
  event.put( iEtaEBEnergySumMinus_ptr, "iEtaEBEnergySumMinus" );
  event.put( iEtaEBETSumPlus_ptr, "iEtaEBETSumPlus" );
  event.put( iEtaEBETSumMinus_ptr, "iEtaEBETSumMinus" );

  event.put( thresholdsEE_ptr, "thresholdsEE" );
  event.put( nEEplus_ptr, "nEEplus" );
  event.put( nEEminus_ptr, "nEEminus" );
  event.put( sumEEEplus_ptr, "sumEEEplus" );
  event.put( sumEEEminus_ptr, "sumEEEminus" );
  event.put( sumETEEplus_ptr, "sumETEEplus" );
  event.put( sumETEEminus_ptr, "sumETEEminus" );
  event.put( iEtaEEMultiplicityPlus_ptr, "iEtaEEMultiplicityPlus" );
  event.put( iEtaEEMultiplicityMinus_ptr, "iEtaEEMultiplicityMinus" );
  event.put( iEtaEEEnergySumPlus_ptr, "iEtaEEEnergySumPlus" );
  event.put( iEtaEEEnergySumMinus_ptr, "iEtaEEEnergySumMinus" );
  event.put( iEtaEEETSumPlus_ptr, "iEtaEEETSumPlus" );
  event.put( iEtaEEETSumMinus_ptr, "iEtaEEETSumMinus" );
}

bool HCALActivitySummary::checkTowerFlagsHB(CaloTower const& tower){ return false; }

bool HCALActivitySummary::checkTowerFlagsHE(CaloTower const& tower){ return false; }

bool HCALActivitySummary::checkTowerFlagsHF(CaloTower const& tower, double emFracThreshold){
  bool flagged = false;

  double emEnergy = tower.emEnergy();
  double hadEnergy = tower.hadEnergy();
  double eTot = emEnergy + hadEnergy;
  double emFrac = fabs(emEnergy/eTot);
  if(eTot > 50.0 && emFrac > emFracThreshold) flagged = true;

  return flagged;  
}

//DEFINE_FWK_MODULE(HCALActivitySummary);
