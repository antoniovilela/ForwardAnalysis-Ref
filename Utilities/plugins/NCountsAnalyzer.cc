
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

class TFileService;
class TH1F;

class NCountsAnalyzer: public edm::EDAnalyzer
{
  public:
    typedef std::map<std::string,TH1F*> HistoMapTH1F;
 
    explicit NCountsAnalyzer(edm::ParameterSet const&);
    ~NCountsAnalyzer();

    void beginJob();
    void analyze(edm::Event const&, edm::EventSetup const&);
  private:
    void bookHistos(HistoMapTH1F&, edm::Service<TFileService> const&);    

    bool useWeight_;
    edm::InputTag weightTag_;

    HistoMapTH1F histosTH1F_;
};

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"

NCountsAnalyzer::NCountsAnalyzer(edm::ParameterSet const& pset): useWeight_(false){

  std::string parName = "weightTag";
  if( pset.exists(parName) ){
     useWeight_ = true; 
     weightTag_ = pset.getParameter<edm::InputTag>("weightTag");
  }
}

NCountsAnalyzer::~NCountsAnalyzer(){}

void NCountsAnalyzer::beginJob(){
  edm::Service<TFileService> fs;

  TH1::SetDefaultSumw2(true);

  bookHistos(histosTH1F_,fs);
}

void NCountsAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){

  histosTH1F_["NCounts"]->Fill(0.);

  if(useWeight_){
     edm::Handle<double> weightH;
     event.getByLabel(weightTag_,weightH);

     double weight = *weightH;
     histosTH1F_["SumWeights"]->Fill(0.,weight);
  }
}

void NCountsAnalyzer::bookHistos(HistoMapTH1F& histos, edm::Service<TFileService> const& fs){
  histos["NCounts"] = fs->make<TH1F>("NCounts","NCounts",1,0,1);
  histos["SumWeights"] = fs->make<TH1F>("SumWeights","SumWeights",1,0,1);
}

DEFINE_FWK_MODULE(NCountsAnalyzer);
