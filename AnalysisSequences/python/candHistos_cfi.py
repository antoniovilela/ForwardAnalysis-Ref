import FWCore.ParameterSet.Config as cms

from candHistograms_cff import *

candHistos = cms.EDAnalyzer('CandHistoAnalyzer',
    src = cms.InputTag(''),
    histograms = cms.VPSet(ptHistogram,etaHistogram)
)
