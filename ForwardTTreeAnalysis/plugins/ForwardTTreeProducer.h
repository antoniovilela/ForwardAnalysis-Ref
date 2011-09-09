#ifndef ForwardTTreeProducer_h
#define ForwardTTreeProducer_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsAnalysis.h"

class DiffractiveEvent;
class ExclusiveDijetsEvent; 
class TTree;

class ForwardTTreeProducer : public edm::EDAnalyzer 
{
  public:
    explicit ForwardTTreeProducer(edm::ParameterSet const&);
    virtual void beginJob();
    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void analyze(edm::Event const&, edm::EventSetup const&);
    virtual void endJob();
    virtual ~ForwardTTreeProducer();

  private:  
    diffractiveAnalysis::DiffractiveAnalysis diffractiveAnalysis_;
    exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis exclusiveDijetsAnalysis_;

    TTree* data_;
    DiffractiveEvent* diffractiveEvent_;
    ExclusiveDijetsEvent* exclusiveDijetsEvent_;
};
#endif
