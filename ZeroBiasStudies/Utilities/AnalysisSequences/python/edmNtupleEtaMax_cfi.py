import FWCore.ParameterSet.Config as cms

from Utilities.AnalysisSequences.edmNtupleCandView_cfi import edmNtupleCandView as edmNtupleEtaMax
edmNtupleEtaMax.src = "etaMaxPFCands"
edmNtupleEtaMax.variables = cms.VPSet( cms.PSet( tag = cms.untracked.string("etaMax"),
                                                 quantity = cms.untracked.string("eta") ) )
