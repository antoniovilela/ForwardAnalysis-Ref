import FWCore.ParameterSet.Config as cms

leadingJets = cms.EDFilter("LargestPtCandViewSelector",
      src = cms.InputTag(""),
      maxNumber = cms.uint32(2)
)
