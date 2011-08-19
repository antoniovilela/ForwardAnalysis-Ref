import FWCore.ParameterSet.Config as cms

#selectGoodTracks = cms.EDFilter("TrackSelector",
 #  src = cms.InputTag("generalTracks"),
  # cut = cms.string("pt > 0.3 & numberOfValidHits > 7 & d0 <= 3.5")
#)

selectGoodTracks = cms.EDFilter("TrackAnalysisSelector",
    src = cms.InputTag("generalTracks"),
    vertexTag = cms.InputTag("offlinePrimaryVertices"),
    ptMin = cms.double(0.5),
    etaMin = cms.double(-2.4),
    etaMax = cms.double(2.4),
    dxySigMax = cms.double(3.0),
    dzSigMax = cms.double(3.0),
    sigPtMax = cms.double(0.01), # sigma(pt)/pt
    quality = cms.string("highPurity")
)
