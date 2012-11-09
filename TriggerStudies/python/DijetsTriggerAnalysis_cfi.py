import FWCore.ParameterSet.Config as cms

"""
DijetsTriggerAnalysis = cms.PSet(
  #gtDigisTag = cms.InputTag("gtDigis"),
  #l1GTObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
  gctDigisTag = cms.InputTag("gctDigis"), 
  # ET sum/tower count per HF ring (HFRingEtSums, HFBitCounts)
  hfRingSumType = cms.string("HFBitCounts"),
  hfRingThreshold = cms.uint32(5),
  l1TriggerNames = cms.vstring('L1_SingleJet36_FwdVeto','L1_SingleJet36','L1_DoubleJet36_Central','L1_SingleEG18','L1_DoubleEG5') 
)
"""

DijetsTriggerAnalysis = cms.PSet(
    gctDigisTag = cms.InputTag("gctDigis"), 
    # ET sum/tower count per HF ring (HFRingEtSums, HFBitCounts)
    hfRingEtSumThreshold = cms.uint32(8),
    hfRingBitCountThreshold = cms.uint32(8),
    l1TriggerNames = cms.vstring('L1_SingleJet36_FwdVeto','L1_SingleJet36','L1_DoubleJet36_Central','L1_DoubleEG5') 
)
