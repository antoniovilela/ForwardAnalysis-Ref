import FWCore.ParameterSet.Config as cms

from Utilities.AnalysisSequences.edmNtupleCandView_cfi import edmNtupleCandView as edmNtupleEtaMin
edmNtupleEtaMin.src = "etaMinPFCands"
edmNtupleEtaMin.variables = cms.VPSet( cms.PSet( tag = cms.untracked.string("etaMin"),
                                                 quantity = cms.untracked.string("eta") ) )
