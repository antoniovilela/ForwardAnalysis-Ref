#include "ForwardTTreeProducer.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsAnalysis.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"

#include "TTree.h"

using namespace diffractiveAnalysis;
using namespace exclusiveDijetsAnalysis;

ForwardTTreeProducer::ForwardTTreeProducer(edm::ParameterSet const& pset):
   diffractiveAnalysis_( pset.getParameter<edm::ParameterSet>("diffractiveAnalysis") ),
   exclusiveDijetsAnalysis_( pset.getParameter<edm::ParameterSet>("exclusiveDijetsAnalysis") ) {}

ForwardTTreeProducer::~ForwardTTreeProducer()
{
  delete diffractiveEvent_;
  delete exclusiveDijetsEvent_;
}

void ForwardTTreeProducer::beginJob() 
{
  edm::Service<TFileService> fs;
  data_ = fs->make<TTree>("ProcessedTree","ProcessedTree");
  diffractiveEvent_ = new DiffractiveEvent();
  exclusiveDijetsEvent_ = new ExclusiveDijetsEvent();
  data_->Branch("DiffractiveAnalysis","DiffractiveEvent",&diffractiveEvent_);
  data_->Branch("ExclusiveDijetsAnalysis","ExclusiveDijetsEvent",&exclusiveDijetsEvent_);
}

void ForwardTTreeProducer::endJob() {}

void ForwardTTreeProducer::beginRun(edm::Run const & run, edm::EventSetup const& setup)
{
  diffractiveAnalysis_.setBeginRun(run,setup);
  exclusiveDijetsAnalysis_.setBeginRun(run,setup);
}

void ForwardTTreeProducer::analyze(edm::Event const& event, edm::EventSetup const& setup) 
{
  diffractiveAnalysis_.fillEventData(*diffractiveEvent_,event,setup); 
  exclusiveDijetsAnalysis_.fillEventData(*exclusiveDijetsEvent_,event,setup); 
  data_->Fill();
}

DEFINE_FWK_MODULE(ForwardTTreeProducer);
