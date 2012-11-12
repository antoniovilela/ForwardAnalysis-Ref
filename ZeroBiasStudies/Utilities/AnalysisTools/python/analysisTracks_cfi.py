import FWCore.ParameterSet.Config as cms

analysisTracks = cms.EDProducer("TrackAnalysisSelector",
    src = cms.InputTag("generalTracks"),
    vertexTag = cms.InputTag("offlinePrimaryVertices"),
    ptMin = cms.double(0.5),
    etaMin = cms.double(-2.4),
    etaMax = cms.double(2.4),
    dxySigMax = cms.double(3.0),
    dzSigMax = cms.double(3.0),
    sigPtMax = cms.double(0.10), # sigma(pt)/pt
    quality = cms.string("highPurity")
)
