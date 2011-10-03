import FWCore.ParameterSet.Config as cms

PATInfo = cms.PSet(
    # Input tags
    #vertexTag       = cms.InputTag("goodOfflinePrimaryVertices"),
    #trackTag        = cms.InputTag("analysisTracks"),
    #jetTag          = cms.InputTag("selectedPatJetsPFlow"),
    #jetMatch        = cms.string("jetTriggerMatchHLTJets"),
    patTriggerEvent = cms.InputTag("patTriggerEvent"),
    patTrigger      = cms.InputTag("patTrigger"),
    L1AlgoBitName   = cms.vstring('L1_ZeroBias','L1_BptxMinus_NotBptxPlus','L1_SingleJet30U'),
    HLTAlgoBitName  = cms.vstring('HLT_ExclDiJet30U_HFOR_v1','HLT_DiJetAve100U_v1')
)
