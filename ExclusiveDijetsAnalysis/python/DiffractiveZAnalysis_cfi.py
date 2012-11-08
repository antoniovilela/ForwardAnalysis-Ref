import FWCore.ParameterSet.Config as cms

from ForwardAnalysis.DiffractiveZAnalysis.pfThresholds_cfi import pfThresholds

DiffractiveZAnalysis = cms.PSet(
    jetTag = cms.InputTag("selectedPatJetsPFlow")
)
