import FWCore.ParameterSet.Config as cms

from pfHistograms_cff import *

pfHistos = cms.EDAnalyzer('PFCandidateHistoAnalyzer',
    src = cms.InputTag('particleFlow'),
    histograms = cms.VPSet(pfPtHistogram,pfEtaHistogram)
)
