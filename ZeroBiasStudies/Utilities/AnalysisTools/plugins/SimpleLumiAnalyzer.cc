
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

class TFileService;
class TH1F;
class TH2F;

class SimpleLumiAnalyzer: public edm::EDAnalyzer
{
  public:
    typedef std::map<std::string,TH1F*> HistoMapTH1F;
    typedef std::map<std::string,TH2F*> HistoMapTH2F;
 
    explicit SimpleLumiAnalyzer(edm::ParameterSet const&);
    ~SimpleLumiAnalyzer();

    void beginJob();
    void analyze(edm::Event const&, edm::EventSetup const&);
  private:
    void bookHistos(HistoMapTH1F&, edm::Service<TFileService> const&);    
    void bookHistos(HistoMapTH2F&, edm::Service<TFileService> const&);

    edm::InputTag vertexTag_;
    edm::InputTag lumiWeightTag_;

    HistoMapTH1F histosTH1F_;
    HistoMapTH2F histosTH2F_;
};

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
#include "TH2F.h"

SimpleLumiAnalyzer::SimpleLumiAnalyzer(edm::ParameterSet const& pset):
  vertexTag_(pset.getParameter<edm::InputTag>("VertexTag")),
  lumiWeightTag_(pset.getParameter<edm::InputTag>("LumiWeightTag")) {}

SimpleLumiAnalyzer::~SimpleLumiAnalyzer(){}

void SimpleLumiAnalyzer::beginJob(){
  edm::Service<TFileService> fs;

  TH1::SetDefaultSumw2(true);

  bookHistos(histosTH1F_,fs);
  bookHistos(histosTH2F_,fs);
}

void SimpleLumiAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::Vertex> > vertexCollection;
  event.getByLabel(vertexTag_,vertexCollection);

  // Find number of good vertices
  int nGoodVertices = 0;
  edm::View<reco::Vertex>::const_iterator vtx = vertexCollection->begin();
  edm::View<reco::Vertex>::const_iterator vtx_end = vertexCollection->end();
  for(; vtx != vtx_end; ++vtx){
     if(!vtx->isValid()) continue; // skip non-valid vertices
     if(vtx->isFake()) continue; // skip vertex from beam spot
     ++nGoodVertices;
  } 

  edm::Handle<double> lumiWeight;
  event.getByLabel(lumiWeightTag_,lumiWeight);
  double lumi = *lumiWeight;

  histosTH1F_["instlumi"]->Fill(lumi);
  histosTH1F_["nVertex"]->Fill(nGoodVertices);
  histosTH2F_["nVertexVsinstlumi"]->Fill(lumi,nGoodVertices); 
}

void SimpleLumiAnalyzer::bookHistos(HistoMapTH1F& histos, edm::Service<TFileService> const& fs){
  histos["instlumi"] = fs->make<TH1F>("instlumi","instlumi",4000,0.,1.);
  histos["nVertex"] = fs->make<TH1F>("nVertex","nVertex",20,0,20);
}

void SimpleLumiAnalyzer::bookHistos(HistoMapTH2F& histos, edm::Service<TFileService> const& fs){
  histos["nVertexVsinstlumi"] = fs->make<TH2F>("nVertexVsinstlumi","nVertexVsinstlumi",4000,0.,1.,20,0,20);
}

DEFINE_FWK_MODULE(SimpleLumiAnalyzer);
