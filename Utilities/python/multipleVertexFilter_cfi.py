import FWCore.ParameterSet.Config as cms

multipleVertexFilter = cms.EDFilter("MultipleVertexFilter",
    src = cms.InputTag('')
    )
