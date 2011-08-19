import FWCore.ParameterSet.Config as cms

#leadingJets = cms.EDProducer("CandViewSelector",
 #     src = cms.InputTag(""),
  #    maxNumber = cms.uint32(2)
#)

leadingJets = cms.EDFilter("LargestPtCandViewSelector",
#cms.EDProducer("LargestPtCandViewSelector",
      src = cms.InputTag(""),
      maxNumber = cms.uint32(2)
)
