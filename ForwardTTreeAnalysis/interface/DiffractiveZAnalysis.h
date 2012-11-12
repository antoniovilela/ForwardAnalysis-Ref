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

      void fillTriggerInfo(DiffractiveZEvent&, const edm::Event&, const edm::EventSetup&);

      edm::InputTag triggerResultsTag_;
      edm::InputTag jetTag_;

      std::vector<std::string> hltPathNames_;
      std::string selectionPathName_;

};

} // namespace
#endif 
