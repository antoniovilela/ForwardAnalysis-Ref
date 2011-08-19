import FWCore.ParameterSet.Config as cms

singleVertexFilter = cms.EDFilter("SingleVertexFilter",
    VertexTag = cms.InputTag("offlinePrimaryVertices")
)
