import FWCore.ParameterSet.Config as cms

from trackHistograms_cff import *

trackHistos = cms.EDAnalyzer('TrackHistoAnalyzer',
    src = cms.InputTag('generalTracks'),
    histograms = cms.VPSet(trackPtHistogram,trackEtaHistogram)
)
