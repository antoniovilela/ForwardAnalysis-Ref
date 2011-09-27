
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATInfo.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATInfoEvent.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

// #include "DataFormats/JetReco/interface/Jet.h"
// #include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
// #include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h" //eliza
//Pat Utils
#include "PhysicsTools/PatUtils/interface/TriggerHelper.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "DataFormats/PatCandidates/interface/TriggerEvent.h"
#include "DataFormats/PatCandidates/interface/TriggerFilter.h"
#include "DataFormats/PatCandidates/interface/TriggerAlgorithm.h"
#include "DataFormats/PatCandidates/interface/TriggerCondition.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"


#include "DataFormats/Common/interface/RefVector.h"
#include "FWCore/Utilities/interface/RegexMatch.h"
//#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"


#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>


//using namespace forwardAnalysis;
using namespace pat;
using patInfo::PATInfo;

PATInfo::PATInfo(const edm::ParameterSet& pset):
  runOnData_(true),

  vertexTag_(pset.getParameter<edm::InputTag>("vertexTag")),
  trackTag_(pset.getParameter<edm::InputTag>("trackTag")),
  jetTag_(pset.getParameter<edm::InputTag>("jetTag")),
 // pat::Trigger
  patTrigger_(pset.getParameter< edm::InputTag >( "patTrigger" ) ),
 // pat::TriggerEvent
  patTriggerEvent_(pset.getParameter< edm::InputTag >( "patTriggerEvent" ) ),
  jetMatch_( pset.getParameter< std::string >( "jetMatch" ) ),
  L1algoBitname_( pset.getParameter<std::vector<std::string> >( "L1AlgoBitName" ) ),
  usePAT_(pset.getUntrackedParameter<bool>("UsePAT",true))

{}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PATInfo::~PATInfo(){}

void PATInfo::setBeginRun(const edm::Run& run, const edm::EventSetup& setup){
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PATInfo::fillEventData(PATInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Reset info
//  eventData.reset();

  runOnData_ = event.isRealData();
  patTriggerInfo(eventData,event,setup);

}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PATInfo::patTriggerInfo(PATInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup)
{ 
  //Ref:http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Bromo/TopAnalysis/TopAnalyzer/src/JetTrigger.cc

  if(usePAT_){
 
  // PAT trigger event
  edm::Handle< TriggerEvent > triggerEvent;
  event.getByLabel( patTriggerEvent_ , triggerEvent );


//  algoBits is defined as a RefVector (think as it was a vector of pointers). It points to TriggerAlgorithm objects
  pat::TriggerAlgorithmRefVector algoBits = triggerEvent->physAlgorithms();
  pat::TriggerAlgorithmRefVector::const_iterator itrBit = algoBits.begin(); 
  
  size_t idxpath = 0;
  std::vector<std::string>::const_iterator l1path =  L1algoBitname_.begin();
  std::vector<std::string>::const_iterator l1paths_end =  L1algoBitname_.end(); 
  
  for(; l1path != l1paths_end; ++l1path,++idxpath){
  int nbit = 0;
  while ( itrBit != algoBits.end()) {
  itrBit++;    

  //std::cout << l1path[++idxpath].compare( (*itrBit)->name() )<<std::endl;


    std::string l1name(l1path[idxpath]);
    std::string l1TriggerName( (*itrBit)->name() );
    //char *l1name_;
    //const l1name_=l1name.c_str();
    //char *l1TriggerName_;
    //const l1TriggerName_=l1TriggerName.c_str();

    //std::cout << l1TriggerName<< std::endl,

  if ( l1name.compare( l1TriggerName ) == 0  )

    //if (l1name.compare( (*itrBit)->name() ) == 0 ){
   std::cout <<" looping over algoBits: " << nbit << '\t' 
            <<" L1 Bit: " << (*itrBit)->techTrigger() << '\t' 
  //          <<" Trigger Name: " <<  (*itrBit)->name() << '\n'
  //          <<" Logical Expression:  " << (*itrBit)->logicalExpression() << '\n'
  //          <<" Trigger Condition Keys:" <<  (*itrBit)->conditionKeys().size() << '\t'
           // <<" GT L1 Result:  " <<  (*itrBit)->gtlResult() << '\t'
         //   <<" Trigger Decision:  " <<  (*itrBit)->decision() << '\t'
    //        <<" Trigger Decision Before Mask: " <<  (*itrBit)->decisionBeforeMask() << '\t'
    //        <<" Trigger Decision After Mask: " << (*itrBit)->decisionAfterMask() << '\t'
            << '\n';


        
          TechL1bit_ = (*itrBit)->techTrigger();  
          //L1LogicalExpression_ = (*itrBit)->logicalExpression(); 
	  //L1TriggerConditionKeys_ = (*itrBit)->conditionKeys().size();
          GTL1Results_ = (*itrBit)->gtlResult();
          L1TriggerDecision_ = (*itrBit)->decision() ;
          L1TriggerDecisionBeforeMask_ = (*itrBit)->decisionBeforeMask();
          L1TriggerDecisionAfterMask_ = (*itrBit)->decisionAfterMask();
         
 
          ///   eventData.SetL1TriggerName();   
              eventData.SetNBit(nbit);
              eventData.SetTechL1Bit(TechL1bit_);
              //eventData.SetL1TriggerConditionKeys(L1TriggerConditionKeys_);
              eventData.SetGTL1Results(GTL1Results_);
              eventData.SetL1TriggerDecision(L1TriggerDecision_);
              eventData.SetL1TriggerDecisionBeforeMask(L1TriggerDecisionBeforeMask_);
              eventData.SetL1TriggerDecisionAfterMask(L1TriggerDecisionAfterMask_);
  

       }//end of selection
    }//L1TriggerName loop

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //Pat Jet
//   edm::Handle<JetCollection> jetCollectionH;
//   event.getByLabel(jetTag_,jetCollectionH);


//   if(jetCollectionH->size() > 1){
// // PAT trigger helper for trigger matching information
//   //const helper::TriggerMatchHelper matchHelper;
//   const pat::TriggerObjectMatch * triggerMatch( triggerEvent->triggerObjectMatchResult(jetMatch_) );

//       for( size_t iJets = 0; iJets < jetCollectionH->size(); ++iJets ) {
//        const reco::CandidateBaseRef candBaseRef( pat::JetRef( jetCollectionH , iJets) );
//        const pat::TriggerObjectRef trigRef(matchHelper.triggerMatchObject(candBaseRef, triggerMatch, event, *triggerEvent ) );
   
//    if ( trigRef.isAvailable()) {
    
//    std::cout << " JetSize: " << jetCollectionH->size() <<std::endl; 
//     double ptTrigCand = candBaseRef->pt();
//     double ptTrigRef = trigRef->pt();
   
//     std::cout<< "ptTrigCand: " << ptTrigCand <<std::endl;
//     std::cout<< "ptTrigRef: " << ptTrigRef <<std::endl;
  
//       eventData.SetPTTriggerCandidate(ptTrigCand);
//       eventData.SetPTTriggerRef(ptTrigRef);
     

//      }
 
//     std::cout<<"PAT Trigger Ref. does not available!!!!!:"<< std::endl;

//    }
//     }//end dijets selection
      }//endUsePAT
       }//end


  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
