import FWCore.ParameterSet.Config as cms

from ForwardAnalysis.DiffractiveZAnalysis.pfThresholds_cfi import pfThresholds

DiffractiveZAnalysis = cms.PSet(
    hltPaths = cms.vstring('HLT_Mu3'),
    TriggerResultsTag = cms.InputTag("TriggerResults::HLT"),
    electronTag = cms.InputTag("gsfElectrons"),
    muonTag = cms.InputTag("muons")
)
