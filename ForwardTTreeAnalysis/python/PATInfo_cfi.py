import FWCore.ParameterSet.Config as cms

PATInfo = cms.PSet(
    # Input tags
    vertexTag       = cms.InputTag("goodOfflinePrimaryVertices"),
    trackTag        = cms.InputTag("analysisTracks"),
    jetTag          = cms.InputTag("selectedPatJetsPFlow"),
    patTriggerEvent = cms.InputTag("patTriggerEvent"),
    patTrigger      = cms.InputTag("patTrigger"),
    jetMatch        = cms.string("jetTriggerMatchHLTJets"),
    L1AlgoBitName   = cms.vstring('L1_BptxMinus_NotBptxPlus','L1_SingleTauJet4'),
    UsePAT          = cms.untracked.bool(True)
)
