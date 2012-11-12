#ifndef Utilities_AnalysisTools_PileUpInfoFilterOld_h
#define Utilities_AnalysisTools_PileUpInfoFilterOld_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class PileUpInfoFilterOld : public edm::EDFilter {
    public:
       explicit PileUpInfoFilterOld(const edm::ParameterSet&);
       ~PileUpInfoFilterOld();

       virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
       edm::InputTag pileUpInfoTag_;
       unsigned int numberPU_;
};

#endif

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

PileUpInfoFilterOld::PileUpInfoFilterOld(const edm::ParameterSet& pset){
  pileUpInfoTag_ = pset.getParameter<edm::InputTag>("pileUpInfoTag"); 
  numberPU_ = pset.getParameter<unsigned int>("numberOfPileUpEvents");
}

PileUpInfoFilterOld::~PileUpInfoFilterOld() {}

bool PileUpInfoFilterOld::filter(edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<PileupSummaryInfo> pileUpInfo;
  event.getByLabel( pileUpInfoTag_, pileUpInfo );
  int n_PU = pileUpInfo->getPU_NumInteractions();

  int requiredPU = numberPU_;
  bool accept = (n_PU == requiredPU);

  return accept;
}

//DEFINE_FWK_MODULE(PileUpInfoFilterOld);
