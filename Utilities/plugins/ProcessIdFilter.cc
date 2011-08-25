
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>

class ProcessIdFilter : public edm::EDFilter {
    public:
       explicit ProcessIdFilter( edm::ParameterSet const& );
       ~ProcessIdFilter();

       virtual void beginJob();
       virtual bool filter( edm::Event&, edm::EventSetup const& );
   private:
       edm::InputTag generatorTag_;

       std::vector<int> selectedProcessIds_;
};

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include <algorithm>

ProcessIdFilter::ProcessIdFilter(edm::ParameterSet const& pset):
  generatorTag_( pset.getParameter<edm::InputTag>("GeneratorTag") ),
  selectedProcessIds_( pset.getParameter<std::vector<int> >("ProcessIds") ) {}

ProcessIdFilter::~ProcessIdFilter() {}

void ProcessIdFilter::beginJob() {}

bool ProcessIdFilter::filter(edm::Event& event, edm::EventSetup const& setup){
  bool accept = false;

  edm::Handle<GenEventInfoProduct> genEventInfo;
  event.getByLabel(generatorTag_,genEventInfo);

  int processId = -1;
  if(genEventInfo.isValid()){
     processId = genEventInfo->signalProcessID();
  } else {
     edm::Handle<edm::HepMCProduct> hepMCProduct;
     event.getByLabel(generatorTag_,hepMCProduct);
     processId = hepMCProduct->getHepMCData().signal_process_id();
  }

  if(std::find(selectedProcessIds_.begin(),selectedProcessIds_.end(),processId) != selectedProcessIds_.end()) accept = true;

  return accept;
}

DEFINE_FWK_MODULE(ProcessIdFilter);
