import FWCore.ParameterSet.Config as cms

leadingJets = cms.EDProducer("LargestPtCandViewSelector",
      src = cms.InputTag(""),
      maxNumber = cms.uint32(2)
)
