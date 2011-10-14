#ifndef PATInfo_h
#define PATInfo_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <map>
#include <vector>

class PATInfoEvent;

namespace patInfo {

   class PATInfo {
      public:
         typedef PATInfoEvent event_type;
         static const char* name;

         PATInfo() {};
	 PATInfo(const edm::ParameterSet&);
	 ~PATInfo();

	 void setBeginRun(const edm::Run&, const edm::EventSetup&);
	 void fillEventData(PATInfoEvent&, const edm::Event&, const edm::EventSetup&);
      private:

	 //L1 PAT information
	 void patL1TriggerInfo(PATInfoEvent&, const edm::Event&, const edm::EventSetup&); //added by SFonseca
	 //HLT PAT information
         void patHLTTriggerInfo(PATInfoEvent&, const edm::Event&, const edm::EventSetup&); //added by SFonseca

         void patL1TriggerInfoSelection(PATInfoEvent&, const edm::Event&, const edm::EventSetup&); //added by SFonseca
         //HLT PAT information
         void patHLTTriggerInfoSelection(PATInfoEvent&, const edm::Event&, const edm::EventSetup&); //added by SFonseca


	 //new input tags:
	 bool runOnData_;
         bool runALLTriggerPath_;
         edm::InputTag patTrigger_;
	 edm::InputTag  patTriggerEvent_;

	 // Jet match objects
	 std::string  jetMatch_;
	 std::vector<std::string>  L1algoBitname_;
	 std::vector<std::string>  HLTalgoBitname_;
	 edm::InputTag vertexTag_;
	 edm::InputTag trackTag_;
	 edm::InputTag metTag_;
	 edm::InputTag jetTag_;
   };
} // namespace
#endif //end code
