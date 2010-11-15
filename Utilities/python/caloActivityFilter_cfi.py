import FWCore.ParameterSet.Config as cms

from ForwardAnalysis.Utilities.caloTowerThresholds_cfi import caloTowerThresholds
from ForwardAnalysis.Utilities.caloActivityThresholds_cfi import caloActivityThresholds

caloActivityFilter = cms.EDProducer("CaloActivityFilter",
    caloTowerThresholds,
    caloActivityThresholds,
    CaloTowerTag = cms.InputTag("towerMaker")
)
