import FWCore.ParameterSet.Config as cms

edmNtupleCandView = cms.EDProducer("CandViewNtpProducer", 
    src = cms.InputTag(""),
    lazyParser = cms.untracked.bool(True),
    prefix = cms.untracked.string(""),
    eventInfo = cms.untracked.bool(True),
    variables = cms.VPSet()  
)
