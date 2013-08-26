#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PFCandInfo.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PFCandInfoEvent.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h" 

//#include "DataFormats/Math/interface/LorentzVector.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Utilities/interface/RegexMatch.h"

#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>

//using namespace forwardAnalysis;
//using namespace pfCandInfo;
using pfCandInfo::PFCandInfo;
const char* PFCandInfo::name = "PFCandInfo";

PFCandInfo::PFCandInfo(const edm::ParameterSet& pset){

	particleTag_= pset.getParameter<edm::InputTag>("particleFlowTag"); 

}
/////////////////////////////////////////////////////////
PFCandInfo::~PFCandInfo(){}

void PFCandInfo::begin() {}

void PFCandInfo::begin(const edm::Run& run, const edm::EventSetup& setup){
}

void PFCandInfo::end() {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void PFCandInfo::fill(PFCandInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

	// Reset info
	eventData.reset();
	//fillPFFlowInfo(eventData,event,setup,PFCandVector);
	fillPFFlowInfo(eventData,event,setup);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//void PFCandInfo::pfCandInfo(PFCandInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup)
//void fillPFFlowInfo(PFCandInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup, std::vector<PFCand>& PFCandVector){
void PFCandInfo::fillPFFlowInfo(PFCandInfoEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

	std::cout << "Using PFCand Info:" << std::endl;

	edm::Handle<reco::PFCandidateCollection> particleFlowCollectionH;

	event.getByLabel("particleFlow",particleFlowCollectionH);
	//reco::PFCandidateCollection const& pflowCollection = *particleFlowCollectionH;

	if( particleFlowCollectionH.isValid() ){

		//PFCandVector.assign(particleFlowCollectionH->size(), PFCand());
		//Int_t j = 0;
		//Double_t charge = 0.;
		/*for(reco::PFCandidateCollection::const_iterator iPFCand_ = particleFlowCollectionH->begin();
		  iPFCand_ != particleFlowCollectionH->end(); ++iPFCand_, ++j){
		  PFCandVector[j].SetPxPyPzE(0. , 0. , 0. , 0.);
		  }*/
		Int_t i = 0;
		for(reco::PFCandidateCollection::const_iterator iPFCand = particleFlowCollectionH->begin();
				iPFCand != particleFlowCollectionH->end();
				++iPFCand, ++i){ //PFCands += pfCand->p4();

			/*PFCandVector[i].SetPxPyPzE(iPFCand->px() , iPFCand->py() , iPFCand->pz() , iPFCand->energy() );
			  std::cout<<"i:"<< i <<"SetPxPyPzE: "<< iPFCand->px() << iPFCand->py() << iPFCand->pz() << iPFCand->energy() <<std::endl;
			  PFCandVector[i].Charge = iPFCand->charge(); std::cout<<"i:"<< i <<"Charge :"<< PFCandVector[i].Charge <<std::endl;
			  PFCandVector[i].ParticleId = static_cast<PFCand::ParticleType>(iPFCand->particleId());

			  PFCand pfCandListInfo;
			  pfCandListInfo.SetPFCandVectorP4(PFCandVector[i].SetPxPyPzE(iPFCand->px() , iPFCand->py() , iPFCand->pz() , iPFCand->energy() ));
			  pfCandListInfo.SetCharge(PFCandVector[i].Charge);
			  pfCandListInfo.SetParticleId(PFCandVector[i].ParticleId);*/

			PFCand pfCand;
			//pfCand.SetPxPyPzE( iPFCand->px() , iPFCand->py() , iPFCand->pz() , iPFCand->energy() );
			
			PFCand::LorentzVector p4( iPFCand->px() , iPFCand->py() , iPFCand->pz() , iPFCand->energy() );

			pfCand.SetPFCandP4( p4 );
			pfCand.SetCharge( iPFCand->charge() );
			pfCand.SetParticleId( static_cast<PFCand::ParticleType>(iPFCand->particleId()) );
			PFCand::LorentzVector const& pfP4 = pfCand.GetPFCandP4();
			std::cout << "i: " << i 
			   << "SetPxPyPzE: "<< pfP4.Px() << ", " 
                                            << pfP4.Py() << ", " << pfP4.Pz() << ", " << pfP4.E() << std::endl;
			std::cout << "i: " << i << "Charge: " << pfCand.GetCharge() << std::endl;
					
			eventData.SetPFCand( pfCand );
		}
	}

}
///////////////////////////////////////////////////////////////////////////////////
