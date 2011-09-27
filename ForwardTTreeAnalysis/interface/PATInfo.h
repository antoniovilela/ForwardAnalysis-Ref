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
     explicit PATInfo(const edm::ParameterSet&);
     ~PATInfo();

     void setBeginRun(const edm::Run&, const edm::EventSetup&);
     void fillEventData(PATInfoEvent&, const edm::Event&, const edm::EventSetup&);
  private:
     
    //PAT information
     void patTriggerInfo(PATInfoEvent&, const edm::Event&, const edm::EventSetup&); //added by SFonseca



//new input tags:
     bool runOnData_;
     edm::InputTag patTrigger_;
     edm::InputTag  patTriggerEvent_;
    
 // Jet match objects
     std::string  jetMatch_;
     std::vector<std::string>  L1algoBitname_;
     edm::InputTag vertexTag_;
     edm::InputTag trackTag_;
     edm::InputTag metTag_;
     edm::InputTag jetTag_;
     bool usePAT_; 

     bool TechL1bit_;  
     bool L1LogicalExpression_ ; 
     bool L1TriggerConditionKeys_ ;
     bool GTL1Results_;
     bool L1TriggerDecision_ ;
     bool L1TriggerDecisionBeforeMask_ ;
     bool L1TriggerDecisionAfterMask_ ;



};

} // namespace
#endif //end code
