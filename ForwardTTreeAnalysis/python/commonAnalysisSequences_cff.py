import FWCore.ParameterSet.Config as cms

##---------------------------------------

from L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskAlgoTrigConfig_cff import *
from L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff import *
es_prefer_l1GtTriggerMaskAlgoTrig = cms.ESPrefer("L1GtTriggerMaskAlgoTrigTrivialProducer","l1GtTriggerMaskAlgoTrig")
es_prefer_l1GtTriggerMaskTechTrig = cms.ESPrefer("L1GtTriggerMaskTechTrigTrivialProducer","l1GtTriggerMaskTechTrig")

from ForwardAnalysis.AnalysisSequences.hltFilter_cfi import *
#hltBscMinBiasORBptxPlusORMinusFilter = hltFilter.clone(HLTPaths = ['HLT_L1_BscMinBiasOR_BptxPlusORMinus'])
##---------------------------------------

from ForwardAnalysis.AnalysisSequences.primaryVertexFilter_cfi import *

from ForwardAnalysis.AnalysisSequences.filterScraping_cfi import *

from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import *

from ForwardAnalysis.Utilities.hcalNoiseFilter_cfi import *

from ForwardAnalysis.Utilities.analysisTracks_cfi import *

from ForwardAnalysis.AnalysisSequences.analysisVertices_cfi import *

from ForwardAnalysis.Utilities.multipleVertexFilter_cfi import *
multipleVertexFilter.src = 'analysisVertices'
multipleVertexVeto = cms.Sequence(~multipleVertexFilter)

from ForwardAnalysis.Utilities.selectTracksAssociatedToPV_cfi import *
selectTracksAssociatedToPV.src = "analysisTracks"
selectTracksAssociatedToPV.vertexTag = "offlinePrimaryVertices"
selectTracksAssociatedToPV.maxDistanceFromVertex = 0.4

from ForwardAnalysis.Utilities.tracksOutsideJets_cfi import *
tracksOutsideJets.src = "selectTracksAssociatedToPV" 
tracksOutsideJets.JetTag = "ak5PFJets"
tracksOutsideJets.JetConeSize = 0.5

from ForwardAnalysis.AnalysisSequences.tracksTransverseRegion_cfi import *
tracksTransverseRegion.src = "selectTracksAssociatedToPV"
tracksTransverseRegion.JetTag = "ak5PFJets"

from ForwardAnalysis.Utilities.trackMultiplicity_cfi import *
trackMultiplicityTransverseRegion = trackMultiplicity.clone( src = "tracksTransverseRegion" ) 
#------------------------------
# Particle flow

# Add EtaMax
from ForwardAnalysis.Utilities.pfCandidateSelector_cfi import pfCandidateSelector as pfCandidateNoiseThresholds
from ForwardAnalysis.Utilities.ExcludeHFEdgesStringCut import ExcludeHFEdgesStringCut
from ForwardAnalysis.Utilities.PFCandidateNoiseStringCut import PFCandidateNoiseStringCut
# Change thresholds here if needed
from ForwardAnalysis.AnalysisSequences.pfThresholds_cfi import pfThresholds
pfStrCut1 = ExcludeHFEdgesStringCut().cut()
pfStrCut2 = PFCandidateNoiseStringCut(pfThresholds).cut()
pfStrCut = '%s & %s' % (pfStrCut1,pfStrCut2)
pfCandidateNoiseThresholds.cut = pfStrCut
# Change to no pile-up collection
#pfCandidateNoiseThresholds.src = "pfNoPileUpPFlow" 

from ForwardAnalysis.Utilities.etaMaxCandViewSelector_cfi import etaMaxCandViewSelector as etaMaxPFCands
from ForwardAnalysis.Utilities.etaMinCandViewSelector_cfi import etaMinCandViewSelector as etaMinPFCands
etaMaxPFCands.src = "pfCandidateNoiseThresholds"
etaMinPFCands.src = "pfCandidateNoiseThresholds"

from ForwardAnalysis.AnalysisSequences.genChargedParticles_cfi import genChargedParticles
from ForwardAnalysis.AnalysisSequences.genStableParticles_cfi import genStableParticles
genStableParticles.cut = 'status = 1 & ( ( pdgId != 2212 ) | ( pdgId == 2212 & abs(pz) < %f ) )' % (0.75*3500.0)
genProtonDissociative = genStableParticles.clone( cut = 'pdgId == 9902210' )
etaMaxGen = etaMaxPFCands.clone(src = "genStableParticles")
etaMinGen = etaMinPFCands.clone(src = "genStableParticles")

from ForwardAnalysis.AnalysisSequences.edmNtupleCandView_cfi import edmNtupleCandView
edmNtupleEtaMax = edmNtupleCandView.clone(src = "etaMaxPFCands")
edmNtupleEtaMax.variables = cms.VPSet( cms.PSet( tag = cms.untracked.string("etaMax"),
                                                 quantity = cms.untracked.string("eta") ) )
edmNtupleEtaMin = edmNtupleCandView.clone(src = "etaMinPFCands")
edmNtupleEtaMin.variables = cms.VPSet( cms.PSet( tag = cms.untracked.string("etaMin"),
                                                 quantity = cms.untracked.string("eta") ) )

edmNtupleMxGen = edmNtupleCandView.clone(src = "genProtonDissociative")
edmNtupleMxGen.variables = cms.VPSet( cms.PSet( tag = cms.untracked.string("Mx"),
                                                quantity = cms.untracked.string("mass") ) )
edmNtupleEtaMaxGen = edmNtupleEtaMax.clone(src = "etaMaxGen")
edmNtupleEtaMinGen = edmNtupleEtaMin.clone(src = "etaMinGen")

from ForwardAnalysis.AnalysisSequences.etaMaxSelector_cfi import etaMaxSelector as etaMaxFilter
etaMaxFilter.src = "etaMaxPFCands"
from ForwardAnalysis.AnalysisSequences.etaMinSelector_cfi import etaMinSelector as etaMinFilter
etaMinFilter.src = "etaMinPFCands"

etaMaxGenFilter = etaMaxFilter.clone(src = "etaMaxGen")
etaMinGenFilter = etaMinFilter.clone(src = "etaMinGen")
#------------------------------

from ForwardAnalysis.Utilities.caloActivityFilter_cfi import caloActivityFilter
caloVetoHFPlus = caloActivityFilter.clone(NTowersMaxHFPlus=0)
caloVetoHFMinus = caloActivityFilter.clone(NTowersMaxHFMinus=0)
caloActivityFilter.EnergyThresholdHB = 1.5
caloActivityFilter.EnergyThresholdHE = 2.0
caloActivityFilter.EnergyThresholdHF = 4.0
'''
from ForwardAnalysis.Utilities.castorActivityFilter_cfi import castorActivityFilter
castorActivityFilter.CastorRecHitTag = "castorRecHitCorrector"
castorActivityFilter.SumEMaxCastor = 250.

castorInvalidDataFilter = cms.EDFilter("CastorInvalidDataFilter")
castorVeto = cms.Sequence(castorInvalidDataFilter + castorActivityFilter)
castorTag = cms.Sequence(castorInvalidDataFilter + ~castorActivityFilter)
'''
##-----------------------------------------------------------------
from ForwardAnalysis.Utilities.hcalActivitySummary_cfi import *
hcalActivitySummary.DiscardFlaggedTowers = False
hcalActivitySummaryScale090 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.90,EnergyScaleFactorHE = 0.90,EnergyScaleFactorHF = 0.90)
hcalActivitySummaryScale092 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.92,EnergyScaleFactorHE = 0.92,EnergyScaleFactorHF = 0.92)
hcalActivitySummaryScale095 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.95,EnergyScaleFactorHE = 0.95,EnergyScaleFactorHF = 0.95)
hcalActivitySummaryScale098 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.98,EnergyScaleFactorHE = 0.98,EnergyScaleFactorHF = 0.98)
hcalActivitySummaryScale102 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.02,EnergyScaleFactorHE = 1.02,EnergyScaleFactorHF = 1.02)
hcalActivitySummaryScale105 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.05,EnergyScaleFactorHE = 1.05,EnergyScaleFactorHF = 1.05)
hcalActivitySummaryScale108 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.08,EnergyScaleFactorHE = 1.08,EnergyScaleFactorHF = 1.08)
hcalActivitySummaryScale110 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.10,EnergyScaleFactorHE = 1.10,EnergyScaleFactorHF = 1.10)

##-----------------------------------------------------------------
# Event selection
offlineSelection = cms.Sequence(primaryVertexFilter + filterScraping + HBHENoiseFilter)
eventSelection = cms.Sequence(offlineSelection)
#eventSelectionHLT = cms.Sequence(hltBscMinBiasORBptxPlusORMinusFilter + offlineSelection)

#-------------------------------------------
# Sequences
vertices = cms.Sequence(analysisVertices)

tracks = cms.Sequence(analysisTracks*
                      selectTracksAssociatedToPV*tracksTransverseRegion)

pfCandidates = cms.Sequence(pfCandidateNoiseThresholds* 
                            etaMaxPFCands+etaMinPFCands)

edmDump = cms.Sequence(trackMultiplicityTransverseRegion+
                       hcalActivitySummary+hcalActivitySummaryScale090+hcalActivitySummaryScale092+
                       hcalActivitySummaryScale095+hcalActivitySummaryScale098+
                       hcalActivitySummaryScale102+hcalActivitySummaryScale105+
                       hcalActivitySummaryScale108+hcalActivitySummaryScale110+
                       edmNtupleEtaMax+edmNtupleEtaMin)
#-------------------------------------------
analysisSequences = cms.Sequence(vertices*tracks*pfCandidates*edmDump)
#-------------------------------------------
