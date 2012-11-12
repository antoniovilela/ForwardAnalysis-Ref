#ifndef Utilities_AnalysisTools_MultipleVertexFilter_h
#define Utilities_AnalysisTools_MultipleVertexFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TH1F;

class MultipleVertexFilter : public edm::EDFilter {
   public:
      explicit MultipleVertexFilter(const edm::ParameterSet&);
      ~MultipleVertexFilter();

      virtual void beginJob();
      virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
      edm::InputTag src_;

      struct {
	 TH1F* h_nVertex_;
      } histos_;
};
#endif

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"

using namespace reco;

MultipleVertexFilter::MultipleVertexFilter(const edm::ParameterSet& pset):
        src_( pset.getParameter<edm::InputTag>("src") ) {}

MultipleVertexFilter::~MultipleVertexFilter(){}

void MultipleVertexFilter::beginJob(){
   edm::Service<TFileService> fs;

   histos_.h_nVertex_ = fs->make<TH1F>("nVertex","Nr. of offline primary vertexes",10,0,10);
}

bool MultipleVertexFilter::filter(edm::Event& event, const edm::EventSetup& setup){
   edm::Handle<edm::View<Vertex> > vertexCollectionH;
   event.getByLabel(src_,vertexCollectionH);
   const edm::View<Vertex>& vtxColl = *(vertexCollectionH.product());

   // Find number of good vertices; match one closest to muon vertex 
   int nGoodVertices = 0;
   for(edm::View<Vertex>::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
      if(!vtx->isValid()) continue; // skip non-valid vertices
      if(vtx->isFake()) continue; // skip vertex from beam spot
      ++nGoodVertices;
   }
   histos_.h_nVertex_->Fill(nGoodVertices);

   LogTrace("Analysis|MultipleVertexFilter") << " Number of reconstructed primary vertices in event: " << nGoodVertices;

   // Select multi-vertex events
   bool accept = true;
   if(nGoodVertices < 2) {
      LogTrace("Analysis|MultipleVertexFilter") << ">>>>> Event has zero or one vertex..";
      accept = false;
   }

   return accept;
}

//DEFINE_FWK_MODULE(MultipleVertexFilter);
