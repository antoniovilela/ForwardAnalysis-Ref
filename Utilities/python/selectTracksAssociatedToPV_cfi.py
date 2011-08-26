import FWCore.ParameterSet.Config as cms

selectTracksAssociatedToPV = cms.EDProducer("TrackAssociatedWithPVSelector",
   src = cms.InputTag("generalTracks"),
   copyExtras = cms.untracked.bool(False),
   copyTrajectories = cms.untracked.bool(False),
   vertexTag = cms.InputTag("offlinePrimaryVertices"),
   minDistanceFromVertex = cms.double(0.0),
   maxDistanceFromVertex = cms.double(0.4)
)
