#ifndef DiffractiveZAnalysis_h
#define DiffractiveZAnalysis_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include <vector>
#include <string>
#include <map>

class DiffractiveZEvent;
class TH1F;
class TH2F;

namespace diffractiveZAnalysis {

class DiffractiveZAnalysis {
   public:
      typedef DiffractiveZEvent event_type;
      static const char* name;

      DiffractiveZAnalysis() {} 
      DiffractiveZAnalysis(const edm::ParameterSet&);
      ~DiffractiveZAnalysis();

      void begin();
      void begin(const edm::Run&, const edm::EventSetup&);
      void fill(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);
      void end();
   private:

      void setTFileService();
      void fillTriggerInfo(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);
      void fillElectronsInfo(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);
      void fillMuonsInfo(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);
      void fillTracksInfo(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);
      void fillGenInfo(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);
      void fillZPat(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);
      void fillDetectorVariables(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);
      void fillVariables(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);
      void fillCastor(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);
      void fillZDC(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);
      void fillDetectorEnergyEtaInfo(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);

      edm::InputTag triggerResultsTag_;
      std::vector<std::string> hltPathNames_;
      edm::InputTag electronTag_;
      edm::InputTag muonTag_;
      edm::InputTag pfTag_;
      edm::InputTag genTag_;
      edm::InputTag PVtxCollectionTag_;
      edm::InputTag castorHitsTag_;
      edm::InputTag zdcHitsTag_;
      bool RunCastor_;
      bool RunZDC_;
      bool RunMC_;
      bool RunZPat_;
      bool EachTower_;
      double pTPFThresholdCharged_;
      double energyPFThresholdBar_;
      double energyPFThresholdEnd_;
      double energyPFThresholdHF_;
      double energyThresholdHB_;
      double energyThresholdHE_;
      double energyThresholdHF_;
      double energyThresholdEB_;
      double energyThresholdEE_;
      double castorThreshold_;
      double fCGeVCastor_;
      edm::InputTag caloTowerTag_; 

      std::string selectionPathName_;

      TH1F *hltTriggerPassHisto_,*hltTriggerNamesHisto_;

};

} // namespace
#endif 
