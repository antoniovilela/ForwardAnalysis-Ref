
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include <string>

class TFile;

class LuminosityWeightProducer : public edm::EDProducer {
public:
  LuminosityWeightProducer( edm::ParameterSet const& );
  ~LuminosityWeightProducer();
private:
  void produce( edm::Event &, edm::EventSetup const& );

  std::string rootFileName_;
  std::string prefix_;
  TFile* rootFile_;
};

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <sstream>
#include "TFile.h"
#include "TH1F.h"

LuminosityWeightProducer::LuminosityWeightProducer(edm::ParameterSet const& pset):
  rootFileName_( pset.getParameter<std::string>("rootFileName") ),
  prefix_( pset.getUntrackedParameter<std::string>("prefix","instLumi") ) { 

  rootFile_ = TFile::Open(rootFileName_.c_str(),"read");
  edm::LogVerbatim("LuminosityWeightProducer") << "[LuminosityWeightProducer]: Opened file " << rootFile_->GetName();

  //produces<double>("luminosityWeight");
  produces<double>(); 
}

LuminosityWeightProducer::~LuminosityWeightProducer(){
  rootFile_->Close();
}

void LuminosityWeightProducer::produce(edm::Event& event, edm::EventSetup const& setup) {

  unsigned int runNumber = event.id().run();
  unsigned int lumiSection = event.luminosityBlock();
  int bunchCrossing = event.bunchCrossing();

  edm::LogVerbatim("LuminosityWeightProducer") << "[LuminosityWeightProducer]: Run, Lumi, Bx= " << runNumber << ", " << lumiSection << ", " << bunchCrossing;

  std::stringstream dirName;
  dirName << "Run" << runNumber;

  std::auto_ptr<double> luminosityWeight( new double(-1.) ); 

  TDirectory* runDir = rootFile_->GetDirectory(dirName.str().c_str());
  if( runDir ){
     std::stringstream histoName;
     histoName << prefix_ << "_" << runNumber << "_" << bunchCrossing;
     TH1F* hist = static_cast<TH1F*>( runDir->Get(histoName.str().c_str()) );
     if( hist ){
        edm::LogVerbatim("LuminosityWeightProducer") << "[LuminosityWeightProducer]: Accessing histogram " << hist->GetName(); 
        double lumi = hist->GetBinContent( hist->FindBin(lumiSection) );
        *luminosityWeight = lumi;
     }
  }
  edm::LogVerbatim("LuminosityWeightProducer") << "[LuminosityWeightProducer]: Lumi weight " << *luminosityWeight; 

  //event.put( luminosityWeight, "luminosityWeight" );
  event.put( luminosityWeight ); 
}

//DEFINE_FWK_MODULE(LuminosityWeightProducer);
