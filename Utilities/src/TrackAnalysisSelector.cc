#include "ForwardAnalysis/Utilities/interface/TrackAnalysisSelector.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

namespace forwardAnalysis {

using namespace reco;

TrackAnalysisSelector::TrackAnalysisSelector(const edm::ParameterSet& pset):
   vertexTag_(pset.getParameter<edm::InputTag>("vertexTag")),
   ptMin_(pset.getParameter<double>("ptMin")),
   etaMin_(pset.getParameter<double>("etaMin")),
   etaMax_(pset.getParameter<double>("etaMax")),
   dxySigMax_(pset.getParameter<double>("dxySigMax")),
   dzSigMax_(pset.getParameter<double>("dzSigMax")),
   sigPtMax_(pset.getParameter<double>("sigPtMax")) {

   quality_ = TrackBase::qualityByName( pset.getParameter<std::string>("quality") );
}

TrackAnalysisSelector::~TrackAnalysisSelector() {}

bool TrackAnalysisSelector::operator()(const Track& track, const edm::Event& event) const {
   edm::Handle<edm::View<Vertex> > vertexCollectionH;
   event.getByLabel(vertexTag_,vertexCollectionH);
  
   bool accept;  

   if ( vertexCollectionH.isValid() && vertexCollectionH->size()>0) {
   const Vertex& primaryVertex = *(vertexCollectionH->begin());
 
   double dxySig = track.dxy(primaryVertex.position())/track.dxyError();
   double dzSig = track.dz(primaryVertex.position())/track.dzError();
   double sigPtRel = track.ptError()/track.pt();
 
   accept = ( track.pt() > ptMin_ &&
                   track.eta() > etaMin_ && track.eta() < etaMax_ &&
                   track.quality(quality_) &&
                   dxySig < dxySigMax_ &&
                   dzSig < dzSigMax_ &&
                   sigPtRel < sigPtMax_);
   }
   else{ accept = false; }
   
   return accept;
   
}

} // namespace
