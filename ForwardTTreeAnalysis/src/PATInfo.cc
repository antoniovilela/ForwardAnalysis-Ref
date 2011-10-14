
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATInfo.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATL1Trigger.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PATHLTTrigger.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

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


#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>

//using namespace forwardAnalysis;
using namespace pat;
//using patInfo::PATInfo;
using namespace patInfo;

const char* PATInfo::name = "PATInfo";

PATInfo::PATInfo(const edm::ParameterSet& pset):
  runOnData_(true),
  runALLTriggerPath_(pset.getUntrackedParameter<bool>("runALLTriggerPath",false)),
  /*vertexTag_(pset.getParameter<edm::InputTag>("vertexTag")),
  trackTag_(pset.getParameter<edm::InputTag>("trackTag")),
  jetTag_(pset.getParameter<edm::InputTag>("jetTag")),
  jetMatch_( pset.getParameter< std::string >( "jetMatch" ) ),*/
  patTrigger_(pset.getParameter< edm::InputTag >( "patTrigger" ) ),
  patTriggerEvent_(pset.getParameter< edm::InputTag >( "patTriggerEvent" ) ),
  L1algoBitname_( pset.getParameter<std::vector<std::string> >( "L1AlgoBitName" ) ),
  HLTalgoBitname_( pset.getParameter<std::vector<std::string> >( "HLTAlgoBitName" ) )
  
{}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PATInfo::~PATInfo(){}

void PATInfo::setBeginRun(const edm::Run& run, const edm::EventSetup& setup){
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PATInfo::fillEventData(PATInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Reset info
  eventData.reset();

  runOnData_ = event.isRealData();
 if (runALLTriggerPath_ == true){ 
 std::cout<<"Selection All Trigger Paths that avaliable in file"<< std::endl;
   patL1TriggerInfo(eventData,event,setup);
  patHLTTriggerInfo(eventData,event,setup);
} else{
  std::cout<< "Using Path Info default"<< std::endl;
  patL1TriggerInfoSelection(eventData,event,setup);
  patHLTTriggerInfoSelection(eventData,event,setup);
}

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PATInfo::patL1TriggerInfo(PATInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup)
{ 
   //Ref:http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Bromo/TopAnalysis/TopAnalyzer/src/JetTrigger.cc
  std::cout<< "Using PAT INFO:"<< std::endl;

   // PAT trigger event
   edm::Handle< TriggerEvent > triggerEvent;
   event.getByLabel( patTriggerEvent_ , triggerEvent );

   //  algoBits is defined as a RefVector (think as it was a vector of pointers). It points to TriggerAlgorithm objects
   pat::TriggerAlgorithmRefVector algoBits = triggerEvent->physAlgorithms();
   pat::TriggerAlgorithmRefVector::const_iterator itrBit = algoBits.begin(); 
   pat::TriggerAlgorithmRefVector::const_iterator itrBit_end = algoBits.end(); 

   //unsigned int nL1algoBitname_ = L1algoBitname_.size();
   int nbit = 0;
   for(; itrBit != itrBit_end; ++itrBit){
      ++nbit;

      //std::string l1TriggerName( (*itrBit)->name() );
      std::string l1TriggerName( (*itrBit)->alias() );
      //std::string l1TriggerNameFake("ALL");
      //std::cout <<"L1algoBitname_: " << L1algoBitname_[0]<< std::endl; 
      //std::cout << l1TriggerName << std::endl;
//     std::cout << l1TriggerNameFake << std::endl;


    //  if( std::find(L1algoBitname_.begin(),L1algoBitname_.end(),l1TriggerName) != L1algoBitname_.end() ) continue;

      //std::cout << L1algoBitname_[0] <<"Trigger Name: " <<  (*itrBit)->name() << std::endl;

   //    std::cout <<" looping over algoBits: " << nbit << '\t' 
// 	 <<" L1 Bit: " << (*itrBit)->techTrigger() << '\t' 
// 	 <<" Trigger Name: " <<  (*itrBit)->name() << '\n'
// 	 <<" Trigger alias: " <<  (*itrBit)->alias() << '\n'
// 	 <<" Logical Expression:  " << (*itrBit)->logicalExpression() << '\n'
// 	 <<" Trigger Condition Keys:" <<  (*itrBit)->conditionKeys().size() << '\t'
// 	 <<" GT L1 Result:  " <<  (*itrBit)->gtlResult() << '\t'
// 	 <<" Trigger Decision:  " <<  (*itrBit)->decision() << '\t'
// 	 <<" Trigger Decision Before Mask: " <<  (*itrBit)->decisionBeforeMask() << '\t'
// 	 <<" Trigger Decision After Mask: " << (*itrBit)->decisionAfterMask() << '\t'
// 	 << '\n';
      

      PATL1Trigger patL1TrigInfo;
      patL1TrigInfo.SetTechL1Bit( (*itrBit)->techTrigger() );
      patL1TrigInfo.SetGTL1Results( (*itrBit)->gtlResult() );
      patL1TrigInfo.SetL1TriggerDecision( (*itrBit)->decision() );
      patL1TrigInfo.SetL1TriggerDecisionBeforeMask( (*itrBit)->decisionBeforeMask() );
      patL1TrigInfo.SetL1TriggerDecisionAfterMask( (*itrBit)->decisionAfterMask() );
      patL1TrigInfo.SetL1TriggerName( (*itrBit)->alias() );
      patL1TrigInfo.SetL1LogicalExpression( (*itrBit)->logicalExpression() );
      patL1TrigInfo.SetL1Prescale( (*itrBit)->prescale() );

      eventData.SetL1Trigger(patL1TrigInfo);





   } // L1 trigger algos
   eventData.SetNBit(nbit);
}//end


//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PATInfo::patL1TriggerInfoSelection(PATInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup)
{ 
   //Ref:http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/Bromo/TopAnalysis/TopAnalyzer/src/JetTrigger.cc

   // PAT trigger event
   edm::Handle< TriggerEvent > triggerEvent;
   event.getByLabel( patTriggerEvent_ , triggerEvent );

   //  algoBits is defined as a RefVector (think as it was a vector of pointers). It points to TriggerAlgorithm objects
   pat::TriggerAlgorithmRefVector algoBits = triggerEvent->physAlgorithms();
   pat::TriggerAlgorithmRefVector::const_iterator itrBit = algoBits.begin(); 
   pat::TriggerAlgorithmRefVector::const_iterator itrBit_end = algoBits.end(); 

   //unsigned int nL1algoBitname_ = L1algoBitname_.size();
   int nbit = 0;
   for(; itrBit != itrBit_end; ++itrBit){
      ++nbit;

      //std::string l1TriggerName( (*itrBit)->name() );
      std::string l1TriggerName( (*itrBit)->alias() );

      //     std::cout << l1TriggerName << std::endl;
      if( std::find(L1algoBitname_.begin(),L1algoBitname_.end(),l1TriggerName) == L1algoBitname_.end() ) continue;

   //    std::cout <<" looping over algoBits: " << nbit << '\t' 
// 	 <<" L1 Bit: " << (*itrBit)->techTrigger() << '\t' 
// 	 <<" Trigger Name: " <<  (*itrBit)->name() << '\n'
// 	 <<" Trigger alias: " <<  (*itrBit)->alias() << '\n'
// 	 <<" Logical Expression:  " << (*itrBit)->logicalExpression() << '\n'
// 	 <<" Trigger Condition Keys:" <<  (*itrBit)->conditionKeys().size() << '\t'
// 	 <<" GT L1 Result:  " <<  (*itrBit)->gtlResult() << '\t'
// 	 <<" Trigger Decision:  " <<  (*itrBit)->decision() << '\t'
// 	 <<" Trigger Decision Before Mask: " <<  (*itrBit)->decisionBeforeMask() << '\t'
// 	 <<" Trigger Decision After Mask: " << (*itrBit)->decisionAfterMask() << '\t'
// 	 << '\n';
      

      PATL1Trigger patL1TrigInfo;
      patL1TrigInfo.SetTechL1Bit( (*itrBit)->techTrigger() );
      patL1TrigInfo.SetGTL1Results( (*itrBit)->gtlResult() );
      patL1TrigInfo.SetL1TriggerDecision( (*itrBit)->decision() );
      patL1TrigInfo.SetL1TriggerDecisionBeforeMask( (*itrBit)->decisionBeforeMask() );
      patL1TrigInfo.SetL1TriggerDecisionAfterMask( (*itrBit)->decisionAfterMask() );
      patL1TrigInfo.SetL1TriggerName( (*itrBit)->alias() );
      patL1TrigInfo.SetL1LogicalExpression( (*itrBit)->logicalExpression() );
      patL1TrigInfo.SetL1Prescale( (*itrBit)->prescale() );

      eventData.SetL1Trigger(patL1TrigInfo);
   } // L1 trigger algos
   eventData.SetNBit(nbit);
}//end

//

void PATInfo::patHLTTriggerInfo(PATInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup)
{ 

  // PAT trigger event
  edm::Handle< TriggerEvent > triggerEvent;
  event.getByLabel( patTriggerEvent_ , triggerEvent );

  pat::TriggerPathRefVector HLTalgoBits = triggerEvent->acceptedPaths();
  pat::TriggerPathRefVector::const_iterator HLT_Bit = HLTalgoBits.begin(); 
  pat::TriggerPathRefVector::const_iterator HLT_Bit_end = HLTalgoBits.end(); 
  
  int nHLTbit = 0;
  for(; HLT_Bit != HLT_Bit_end; ++HLT_Bit){
  nHLTbit++ ;
  

     std::string HLTTriggerName( (*HLT_Bit)->name() );

    

     //if( std::find( HLTalgoBitname_.begin(),HLTalgoBitname_.end(),HLTTriggerName) != HLTalgoBitname_.end() ) continue;
     //     std::cout << " HLT Name : " << HLTTriggerName << std::endl;

     //  std::string AcceptedL1SeedName((*HLT_Bit)->acceptedL1Seeds() );

        PATHLTTrigger patHLTTrigInfo;
       
        patHLTTrigInfo.SetHLTPrescale((*HLT_Bit)->prescale());
        patHLTTrigInfo.SetHLTWasRun((*HLT_Bit)->wasRun());
        patHLTTrigInfo.SetHLTWasAccept((*HLT_Bit)->wasAccept());
        patHLTTrigInfo.SetHLTWasError((*HLT_Bit)->wasError());
        patHLTTrigInfo.SetHLTName(HLTTriggerName);

	//  patHLTTrigInfo.SetAcceptedL1SeedName( AcceptedL1SeedName );
	//  patHLTTrigInfo.SetFailedL1SeedName((*HLT_Bit)->failedL1Seeds());

        eventData.SetHLTTrigger(patHLTTrigInfo);
     }//end for
     eventData.SetNHLTBit(nHLTbit);
}//end


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PATInfo::patHLTTriggerInfoSelection(PATInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup)
{ 

  // PAT trigger event
  edm::Handle< TriggerEvent > triggerEvent;
  event.getByLabel( patTriggerEvent_ , triggerEvent );

  pat::TriggerPathRefVector HLTalgoBits = triggerEvent->acceptedPaths();
  pat::TriggerPathRefVector::const_iterator HLT_Bit = HLTalgoBits.begin(); 
  pat::TriggerPathRefVector::const_iterator HLT_Bit_end = HLTalgoBits.end(); 
  
  int nHLTbit = 0;
  for(; HLT_Bit != HLT_Bit_end; ++HLT_Bit){
  nHLTbit++ ;
  

     std::string HLTTriggerName( (*HLT_Bit)->name() );

    

     if( std::find( HLTalgoBitname_.begin(),HLTalgoBitname_.end(),HLTTriggerName) == HLTalgoBitname_.end() ) continue;
     //     std::cout << " HLT Name : " << HLTTriggerName << std::endl;

     //  std::string AcceptedL1SeedName((*HLT_Bit)->acceptedL1Seeds() );

        PATHLTTrigger patHLTTrigInfo;
       
        patHLTTrigInfo.SetHLTPrescale((*HLT_Bit)->prescale());
        patHLTTrigInfo.SetHLTWasRun((*HLT_Bit)->wasRun());
        patHLTTrigInfo.SetHLTWasAccept((*HLT_Bit)->wasAccept());
        patHLTTrigInfo.SetHLTWasError((*HLT_Bit)->wasError());
        patHLTTrigInfo.SetHLTName(HLTTriggerName);

	//  patHLTTrigInfo.SetAcceptedL1SeedName( AcceptedL1SeedName );
	//  patHLTTrigInfo.SetFailedL1SeedName((*HLT_Bit)->failedL1Seeds());

        eventData.SetHLTTrigger(patHLTTrigInfo);
     }//end for
     eventData.SetNHLTBit(nHLTbit);
}//end




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
