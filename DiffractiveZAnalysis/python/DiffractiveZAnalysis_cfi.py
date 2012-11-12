import FWCore.ParameterSet.Config as cms

from ForwardAnalysis.DiffractiveZAnalysis.pfThresholds_cfi import pfThresholds

DiffractiveZAnalysis = cms.PSet(
    TriggerResultsTag = cms.InputTag("TriggerResults::HLT"),
    jetTag = cms.InputTag("selectedPatJetsPFlow")
)
