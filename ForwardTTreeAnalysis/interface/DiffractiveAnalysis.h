#ifndef DiffractiveAnalysis_h
#define DiffractiveAnalysis_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include <vector>
#include <string>
#include <map>

class DiffractiveEvent;

namespace diffractiveAnalysis {

class DiffractiveAnalysis {
   public:
      typedef DiffractiveEvent event_type;
      static const char* name;

      DiffractiveAnalysis() {} 
      DiffractiveAnalysis(const edm::ParameterSet&);
      ~DiffractiveAnalysis();

      void begin();
      void begin(const edm::Run&, const edm::EventSetup&);
      void fill(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void end();
   private:
      void fillEventInfo(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void fillNoiseInfo(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void fillTriggerInfo(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void fillVertexInfo(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void fillTrackInfo(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void fillMETInfo(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void fillJetInfo(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void fillCaloTowerInfo(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void fillCastorInfo(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void fillZDCInfo(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void fillDiffVariables(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
      void fillGenInfo(DiffractiveEvent&, const edm::Event&, const edm::EventSetup&);
 
      void setGenInfo(reco::GenParticleCollection const&, double,
                                                          math::XYZTLorentzVector&,
                                                          math::XYZTLorentzVector&,
                                                          math::XYZTLorentzVector&,
                                                          math::XYZTLorentzVector&,
                                                          math::XYZTLorentzVector&,
                                                          math::XYZTLorentzVector&,
                                                          math::XYZTLorentzVector&,
                                                          math::XYZTLorentzVector&,
                                                          math::XYZTLorentzVector&,
                                                          math::XYZTLorentzVector&);
 
      double MassDissGen(reco::GenParticleCollection const& genParticles, double rangeEtaMin = -999.,
                                                                          double rangeEtaMax = 999.);

      unsigned int nHCALiEta(const std::map<unsigned int, std::vector<unsigned int> >&, 
                             unsigned int, unsigned int);

      double sumEHCALiEta(const std::map<unsigned int, std::vector<double> >&, 
                          unsigned int, unsigned int);
      
      template <class PartColl>
      double MassColl(PartColl& partCollection, double ptThreshold = -1.,
                double energyHBThreshold = -1., double energyHEThreshold = -1.,
                double energyHFThreshold = -1., double energyScale = -1.);

      template <class Coll>
      std::pair<double,double> Xi(Coll& partCollection, double Ebeam, double ptThreshold = -1.,
                                  double energyHBThreshold = -1., double energyHEThreshold = -1.,
                                  double energyHFThreshold = -1., double energyScale = -1.);

      template <class Coll>
      std::pair<double,double> EPlusPz(Coll& partCollection, double ptThreshold = -1.,
                                       double energyHBThreshold = -1., double energyHEThreshold = -1.,
                                       double energyHFThreshold = -1., double energyScale = -1.);

      bool accessCastorInfo_;
      bool accessZDCInfo_;
  
      edm::InputTag vertexTag_;
      edm::InputTag trackTag_;
      edm::InputTag metTag_;
      edm::InputTag jetTag_;
      edm::InputTag caloTowerTag_;
      edm::InputTag castorRecHitTag_;
      edm::InputTag zdcHitsTag_;
      double castorThreshold_;
      double fCGeVCastor_;
      edm::InputTag particleFlowTag_;
      edm::InputTag genChargedTag_;
      edm::InputTag triggerResultsTag_;
      edm::InputTag hcalTowerSummaryTag_;

      double energyThresholdHB_;
      double energyThresholdHE_;
      double energyThresholdHF_;
      double comEnergy_,Ebeam_;
      bool applyEnergyScaleHCAL_;
      double energyScaleHCAL_;
      bool accessMCInfo_;
      std::string hltPathName_;
      //std::vector<std::string> > hltPathNames_;
      int ttBit_; 
};

} // namespace
#endif 
