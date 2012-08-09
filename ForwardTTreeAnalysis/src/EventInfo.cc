
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfo.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"

#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "DataFormats/Luminosity/interface/LumiDetails.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

//using namespace forwardAnalysis;
using eventInfo::EventInfo;

const char* EventInfo::name = "EventInfo";

EventInfo::EventInfo(const edm::ParameterSet& pset):
  runOnData_(pset.getUntrackedParameter<bool>("RunOnData")),
  runWithMCPU_(pset.getUntrackedParameter<bool>("RunWithMCPU")),
  runWithGen_(pset.getUntrackedParameter<bool>("RunWithGen")) {} 

EventInfo::~EventInfo() {}

void EventInfo::begin() {}

void EventInfo::begin(const edm::Run& run, const edm::EventSetup& setup) {}

void EventInfo::end() {}

void EventInfo::fill(EventInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
	  // Reset info
	  eventData.reset();

	  runOnData_ = event.isRealData();
	   
	  fillEventInfo(eventData,event,setup);

	  if( runWithGen_ ){
	     edm::Handle<GenEventInfoProduct> genEventInfoH;
	     event.getByLabel("generator", genEventInfoH);
	     eventData.SetPthat( genEventInfoH->binningValues()[0] );
	     eventData.SetGeneratorWeight( genEventInfoH->weight() );

	     fillPileUpInfo(eventData,event,setup);
	  } else {
	     eventData.SetPthat( -1. );
	     eventData.SetGeneratorWeight( -1. );
	  }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EventInfo::fillEventInfo(EventInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

	  unsigned int eventNumber = event.id().event();
	  unsigned int runNumber = event.id().run();
	  unsigned int lumiSection = event.luminosityBlock();
	  int bunchCrossing = event.bunchCrossing();
	  int orbitNumber   = event.orbitNumber();
	  eventData.SetEventNumber(eventNumber);
	  eventData.SetRunNumber(runNumber);
	  eventData.SetLumiSection(lumiSection);
	  eventData.SetBunchCrossing(bunchCrossing);
	  eventData.SetOrbitNumber(orbitNumber);


	if( runOnData_ ){


			  /////////////////////////////
			  // Instant. luminosity of a lumisection = (delivered luminosity)/(lumisection size=23.36s) 
			  edm::LuminosityBlock const& lumiBlock = event.getLuminosityBlock();
			  edm::Handle<LumiSummary> s;
			  lumiBlock.getByLabel("lumiProducer",s);
			  float instLumiLS=-10.;
			  if( s.isValid() ){
			     instLumiLS=s->avgInsDelLumi(); // calibrated
			     eventData.SetInstDelLumiLS(instLumiLS);
			  } else{
			     eventData.SetInstDelLumiLS(-999.);
			  }

			  // Inst. luminosity per bunch crossing
			  // In EDM it is uncalibrated. For 7TeV collisions calibration constant is 6.37.
			  edm::Handle<LumiDetails> d;
			  lumiBlock.getByLabel("lumiProducer",d);
			  float instLumiBunchOCC1=-10.;
			  if( d.isValid() ){
			     instLumiBunchOCC1 = d->lumiValue(LumiDetails::kOCC1,event.bunchCrossing());
			     instLumiBunchOCC1 = instLumiBunchOCC1*6.37;
			     eventData.SetInstLumiBunch(instLumiBunchOCC1);
			  } else{
			     eventData.SetInstLumiBunch(-9999.);
			  }

		  } 
		  
		  else{
		     eventData.SetInstDelLumiLS(-999.);
		     eventData.SetInstLumiBunch(-9999.);
		  }



}

void EventInfo::fillPileUpInfo(EventInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

	 if(runWithMCPU_){

		  edm::Handle<std::vector<PileupSummaryInfo> >  PupInfo;
		  event.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);

		  std::vector<PileupSummaryInfo>::const_iterator PVI;

		  int nm1 = -1; int n0 = -1; int np1 = -1;
		  float nTrueInt = -1;
		  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {

		     int BX = PVI->getBunchCrossing();

		     if(BX == -1) { 
		       nm1 = PVI->getPU_NumInteractions();
		     }
		     if(BX == 0) { 
		       n0 = PVI->getPU_NumInteractions();
		       nTrueInt = PVI->getTrueNumInteractions(); 
		     }
		     if(BX == 1) { 
		       np1 = PVI->getPU_NumInteractions();
		     }

		  }
		  eventData.SetNPileUpBxm1(nm1);
		  eventData.SetNPileUpBx0(n0);
		  eventData.SetNPileUpBxp1(np1);
		  eventData.SetTrueNumInteractions(nTrueInt);

	}

	else{
	     eventData.SetNPileUpBxm1(-1);
	     eventData.SetNPileUpBx0(-1);
	     eventData.SetNPileUpBxp1(-1);
	     eventData.SetTrueNumInteractions(-1.);
	}

}
