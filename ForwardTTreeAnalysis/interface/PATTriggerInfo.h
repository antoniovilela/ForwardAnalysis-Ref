#ifndef PATTriggerInfo_h
#define PATTriggerInfo_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <map>
#include <vector>

class PATTriggerInfoEvent;

namespace patTriggerInfo {

   class PATTriggerInfo {
      public:
         typedef PATTriggerInfoEvent event_type;
         static const char* name;

         PATTriggerInfo() {};
	 PATTriggerInfo(const edm::ParameterSet&);
	 ~PATTriggerInfo();

	 void begin();
	 void begin(const edm::Run&, const edm::EventSetup&);
	 void fill(PATTriggerInfoEvent&, const edm::Event&, const edm::EventSetup&);
	 void end();
      private:

	 //L1 PAT information
	 void patL1TriggerInfo(PATTriggerInfoEvent&, const edm::Event&, const edm::EventSetup&); //added by SFonseca
	 //HLT PAT information
         void patHLTTriggerInfo(PATTriggerInfoEvent&, const edm::Event&, const edm::EventSetup&); //added by SFonseca

         void patL1TriggerInfoSelection(PATTriggerInfoEvent&, const edm::Event&, const edm::EventSetup&); //added by SFonseca
         //HLT PAT information
         void patHLTTriggerInfoSelection(PATTriggerInfoEvent&, const edm::Event&, const edm::EventSetup&); //added by SFonseca


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
