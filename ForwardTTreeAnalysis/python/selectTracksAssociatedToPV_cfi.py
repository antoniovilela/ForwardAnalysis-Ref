import FWCore.ParameterSet.Config as cms

selectTracksAssociatedToPV = cms.EDProducer("TrackAssociatedWithPVSelector",
   src = cms.InputTag("generalTracks"),
   copyExtras = cms.untracked.bool(False),
   copyTrajectories = cms.untracked.bool(False),
   VertexTag = cms.InputTag("offlinePrimaryVertices"),
   MinDistanceFromVertex = cms.double(0.0),
   MaxDistanceFromVertex = cms.double(0.4)
)
