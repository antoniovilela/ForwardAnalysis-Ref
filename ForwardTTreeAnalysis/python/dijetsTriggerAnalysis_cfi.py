import FWCore.ParameterSet.Config as cms

"""
#dijetsTriggerAnalysis = cms.EDAnalyzer("DijetsTriggerAnalyzer",
  #gtDigisTag = cms.InputTag("gtDigis"),
  #l1GTObjectMapTag = cms.InputTag("hltL1GtObjectMap"),
  gctDigisTag = cms.InputTag("gctDigis"), 
  # ET sum/tower count per HF ring (HFRingEtSums, HFBitCounts)
  hfRingSumType = cms.string("HFBitCounts"),
  hfRingThreshold = cms.uint32(5),
  l1TriggerNames = cms.vstring('L1_SingleJet36_FwdVeto','L1_SingleJet36','L1_DoubleJet36_Central','L1_SingleEG18','L1_DoubleEG5') 
)
"""
#dijetsTriggerAnalysis = cms.EDAnalyzer("ForwardTTreeProducer",
DijetsTriggerAnalysis = cms.PSet(
  gctDigisTag = cms.InputTag("gctDigis"), 
# ET sum/tower count per HF ring (HFRingEtSums, HFBitCounts)
  hfRingEtSumThreshold = cms.uint32(8),
  hfRingBitCountThreshold = cms.uint32(8),
  jetTag = cms.InputTag("hltAntiKT5CaloJets"),
  particleFlowTag = cms.InputTag("hltAntiKT5PFJets"),
  caloTowerTag = cms.InputTag("hltTowerMakerForAll"), 
  #hcalTowerSummaryTag = cms.InputTag("hcalActivitySummary"),
  # Misc
  #energyThresholdHB = cms.double(1.5),
  #energyThresholdHE = cms.double(2.0),
  #energyThresholdHF = cms.double(4.0),
  #comEnergy = cms.double(8000.),
  #applyEnergyScaleHCAL = cms.bool(False),
  #energyScaleFactorHCAL = cms.double(1.0), 
  l1TriggerNames = cms.vstring('L1_SingleJet36','L1_SingleJet16','L1_DoubleJetC56'),
  TriggerResultsTag = cms.InputTag("TriggerResults::HLT"),
  hltPaths = cms.vstring('HLT_ExclDiJet80_HFAND_v*','HLT_ExclDiJet35_HFAND_v*','HLT_ExclDiJet35_HFOR_v*','HLT_PFJet40_v*','HLT_L1SingleJet16_v*','HLT_DiPFJetAve80_v*','HLT_L1SingleJet36_v*') 
  # ('L1_SingleJet36_FwdVeto','L1_SingleJet36','L1_DoubleJet36_Central','L1_DoubleEG5') 
)
#	)
