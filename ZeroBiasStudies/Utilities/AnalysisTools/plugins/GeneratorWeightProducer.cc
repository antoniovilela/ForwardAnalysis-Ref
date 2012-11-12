
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"

class GeneratorWeightProducer : public edm::EDProducer {
public:
  GeneratorWeightProducer( edm::ParameterSet const& );
  ~GeneratorWeightProducer();
private:
  void produce( edm::Event &, edm::EventSetup const& );
  edm::InputTag generatorTag_;
};

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

GeneratorWeightProducer::GeneratorWeightProducer(edm::ParameterSet const& pset):
  generatorTag_( pset.getParameter<edm::InputTag>("generator") ){

  produces<double>(); 
}

GeneratorWeightProducer::~GeneratorWeightProducer(){}

void GeneratorWeightProducer::produce(edm::Event& event, edm::EventSetup const& setup) {

  std::auto_ptr<double> weight( new double(-1.) );

  edm::Handle<GenEventInfoProduct> genEventInfo; 
  event.getByLabel(generatorTag_, genEventInfo);
  if( genEventInfo.isValid() ){
     double gen_weight = genEventInfo->weight();
     *weight = gen_weight;
  }
  edm::LogVerbatim("GeneratorWeightProducer") << "[GeneratorWeightProducer]: Gen weight " << *weight; 

  event.put( weight ); 
}

DEFINE_FWK_MODULE(GeneratorWeightProducer);
