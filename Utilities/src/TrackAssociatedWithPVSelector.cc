#include "ForwardAnalysis/Utilities/interface/TrackAssociatedWithPVSelector.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

using namespace reco;
using forwardAnalysis::TrackAssociatedWithPVSelector;

TrackAssociatedWithPVSelector::TrackAssociatedWithPVSelector(const edm::ParameterSet& pset):
   vertexTag_(pset.getParameter<edm::InputTag>("vertexTag")),
   deltaZVtxMin_(pset.getParameter<double>("minDistanceFromVertex")),
   deltaZVtxMax_(pset.getParameter<double>("maxDistanceFromVertex")){}

TrackAssociatedWithPVSelector::~TrackAssociatedWithPVSelector() {}

bool TrackAssociatedWithPVSelector::operator()(const reco::Track& track, const edm::Event& event) const {
   edm::Handle<edm::View<Vertex> > vertexCollectionH;
   event.getByLabel(vertexTag_,vertexCollectionH);
   const edm::View<Vertex>& vtxColl = *(vertexCollectionH.product());
 
   // Access primary vertex
   const Vertex& primaryVertex = vtxColl.front();
   bool goodPrimaryVertex = ((primaryVertex.isValid())&&(!primaryVertex.isFake()));

   if(!goodPrimaryVertex) return false;

   const math::XYZPoint& trackVtxPos = track.vertex();
   const math::XYZPoint& primVtxPos = primaryVertex.position();

   math::XYZVector dist = trackVtxPos - primVtxPos; 
   bool accept = (fabs(dist.z()) >= deltaZVtxMin_)&&(fabs(dist.z()) <= deltaZVtxMax_);
  
   return accept;
}
