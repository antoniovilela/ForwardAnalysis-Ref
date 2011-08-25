#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class PileUpInfoFilter : public edm::EDFilter {
    public:
       explicit PileUpInfoFilter(const edm::ParameterSet&);
       ~PileUpInfoFilter();

       virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
       edm::InputTag pileUpInfoTag_;
       unsigned int numberPU_;
};

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

PileUpInfoFilter::PileUpInfoFilter(const edm::ParameterSet& pset){
  pileUpInfoTag_ = pset.getParameter<edm::InputTag>("pileUpInfoTag"); 
  numberPU_ = pset.getParameter<unsigned int>("numberOfPileUpEvents");
}

PileUpInfoFilter::~PileUpInfoFilter() {}

bool PileUpInfoFilter::filter(edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<PileupSummaryInfo> pileUpInfo;
  event.getByLabel( pileUpInfoTag_, pileUpInfo );
  int n_PU = pileUpInfo->getPU_NumInteractions();

  int requiredPU = numberPU_;
  bool accept = (n_PU == requiredPU);

  return accept;
}

DEFINE_FWK_MODULE(PileUpInfoFilter);
