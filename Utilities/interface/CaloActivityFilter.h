#ifndef ForwardAnalysisUtilities_CaloActivityFilter_h
#define ForwardAnalysisUtilities_CaloActivityFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class CaloTower;

namespace forwardAnalysis {

class DefaultCaloTowerSelector {
   public:
      DefaultCaloTowerSelector(edm::ParameterSet const& pset) {}
      ~DefaultCaloTowerSelector() {}

      bool operator() (edm::Event const& event, CaloTower const&){ return true; }
};

template <class Selector=DefaultCaloTowerSelector>
class CaloActivityFilter : public edm::EDFilter {
    public:
       explicit CaloActivityFilter( edm::ParameterSet const& );
       ~CaloActivityFilter();

       virtual void beginJob();
       virtual bool filter( edm::Event&, edm::EventSetup const& );
   private:
       Selector selector_;

       edm::InputTag caloTowerTag_; 

       double energyThresholdHB_;
       double energyThresholdHE_;
       double energyThresholdHF_;
       double energyThresholdEB_;
       double energyThresholdEE_;

       int nHBTowersMaxPlus_,nHBTowersMaxMinus_;
       int nHETowersMaxPlus_,nHETowersMaxMinus_;
       int nHFTowersMaxPlus_,nHFTowersMaxMinus_; 
       int nEBTowersMaxPlus_,nEBTowersMaxMinus_;
       int nEETowersMaxPlus_,nEETowersMaxMinus_;

       double sumEHBMaxPlus_,sumEHBMaxMinus_; 
       double sumEHEMaxPlus_,sumEHEMaxMinus_; 
       double sumEHFMaxPlus_,sumEHFMaxMinus_;
       double sumEEBMaxPlus_,sumEEBMaxMinus_;
       double sumEEEMaxPlus_,sumEEEMaxMinus_;

       double sumETHBMaxPlus_,sumETHBMaxMinus_;
       double sumETHEMaxPlus_,sumETHEMaxMinus_;
       double sumETHFMaxPlus_,sumETHFMaxMinus_; 
       double sumETEBMaxPlus_,sumETEBMaxMinus_;
       double sumETEEMaxPlus_,sumETEEMaxMinus_;
};
} // namespace
// Implementation
#include "CaloActivityFilter.icc"

#endif
