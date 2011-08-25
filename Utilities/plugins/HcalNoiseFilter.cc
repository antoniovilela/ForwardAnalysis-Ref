#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class HcalNoiseFilter : public edm::EDFilter {
    public:
       explicit HcalNoiseFilter( edm::ParameterSet const& );
       ~HcalNoiseFilter();

       virtual bool filter( edm::Event&, edm::EventSetup const& );
   private:
       bool loose_,tight_; 
};

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/METReco/interface/HcalNoiseSummary.h"

HcalNoiseFilter::HcalNoiseFilter(edm::ParameterSet const& pset): loose_(true),tight_(true) {}

HcalNoiseFilter::~HcalNoiseFilter() {}

bool HcalNoiseFilter::filter(edm::Event& event, edm::EventSetup const& setup){

  edm::Handle<HcalNoiseSummary> noiseSummaryH;
  event.getByLabel("hcalnoise",noiseSummaryH);   

  bool passNoiseLoose = noiseSummaryH->passLooseNoiseFilter();
  bool passNoiseTight = noiseSummaryH->passTightNoiseFilter();

  bool accept = true;
  if(loose_ && !passNoiseLoose) accept = false;
  if(tight_ && !passNoiseTight) accept = false;
  
  return accept;
}

DEFINE_FWK_MODULE(HcalNoiseFilter);
