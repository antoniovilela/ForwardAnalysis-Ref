#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>

class HepMCFileReader;

namespace HepMC{
  class GenEvent;
}

class MCFileReader : public edm::EDProducer {
   public:
      MCFileReader(const edm::ParameterSet &);
      virtual ~MCFileReader();
   
   private:
      void produce(edm::Event&, const edm::EventSetup&);
      void endRun(edm::Run&, edm::EventSetup const&); 
 
      std::vector<std::string> fileNames_;
      HepMCFileReader *reader_;
      HepMC::GenEvent *evt_;
};

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "ForwardAnalysis/Utilities/interface/HepMCFileReader.h" 
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

using namespace edm;
using namespace std;

MCFileReader::MCFileReader(const ParameterSet & pset):
   fileNames_( pset.getUntrackedParameter<vector<string> >("fileNames") ),
   reader_(HepMCFileReader::instance()), evt_(0)
{
   LogInfo("MCFileReader") << "Reading HepMC file:" << fileNames_[0];
   string fileName = fileNames_[0];
   // strip the file: 
   if (fileName.find("file:") == 0){
      fileName.erase(0,5);
   }  

   reader_->initialize(fileName);  
   produces<HepMCProduct>();
   produces<GenEventInfoProduct>();
   produces<GenRunInfoProduct, edm::InRun>();
}

MCFileReader::~MCFileReader(){}

void MCFileReader::produce(Event & event, const EventSetup& setup){
   auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  

   LogInfo("MCFileSource") << "Start Reading";
   evt_ = reader_->fillCurrentEventData(); 
   if (evt_ == 0) return;

   bare_product->addHepMCData(evt_);
   event.put(bare_product);

   auto_ptr<GenEventInfoProduct> genEventInfo( new GenEventInfoProduct(evt_) );
   event.put(genEventInfo);
}

void MCFileReader::endRun(Run& run, EventSetup const& setup) {

   auto_ptr<GenRunInfoProduct> genRunInfoProd( new GenRunInfoProduct() );
   genRunInfoProd->setInternalXSec(-1.);
   genRunInfoProd->setFilterEfficiency(-1.);
   genRunInfoProd->setExternalXSecLO(-1.);
   genRunInfoProd->setExternalXSecNLO(-1.);   

   run.put(genRunInfoProd);
}
DEFINE_FWK_MODULE(MCFileReader);
