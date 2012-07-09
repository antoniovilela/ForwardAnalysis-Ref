import FWCore.ParameterSet.Config as cms

##---------------------------------------
from ForwardAnalysis.AnalysisSequences.hltLevel1GTSeed_cff import *
bptx = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('0'))
l1Tech4 = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('4'))

from ForwardAnalysis.AnalysisSequences.hltFilter_cfi import *
hltMinBias = hltFilter.clone(HLTPaths = ['HLT_L1_BscMinBiasOR_BptxPlusORMinus'])
hltZeroBias = hltFilter.clone(HLTPaths = ['HLT_ZeroBias'])
#hltMinBias = hltFilter.clone(HLTPaths = ['HLT_L1Tech53_MB_2_v1'])
#hltZeroBias = hltFilter.clone(HLTPaths = ['HLT_L1Tech54_ZeroBias_v1'])

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

#from ForwardAnalysis.Utilities.selectTracksAssociatedToPV_cfi import *
#selectTracksAssociatedToPV.src = "analysisTracks"
#selectTracksAssociatedToPV.vertexTag = "analysisVertices"
#selectTracksAssociatedToPV.maxDistanceFromVertex = 0.4

#------------------------------
# Particle flow
from ForwardAnalysis.Utilities.pfCandidateSelector_cfi import pfCandidateSelector as pfCandidateNoiseThresholds
from ForwardAnalysis.Utilities.ExcludeHFEdgesStringCut import ExcludeHFEdgesStringCut
from ForwardAnalysis.Utilities.PFCandidateNoiseStringCut import PFCandidateNoiseStringCut
# Change thresholds here if needed
from ForwardAnalysis.AnalysisSequences.pfThresholds_cfi import pfThresholds

pfStrCutHFEdges = ExcludeHFEdgesStringCut().cut()
pfStrCutNoise = PFCandidateNoiseStringCut(pfThresholds).cut()
pfStrCut = '%s & %s' % (pfStrCutHFEdges,pfStrCutNoise)
pfCandidateNoiseThresholds.cut = pfStrCut
pfCandidateHFEdges = pfCandidateNoiseThresholds.clone( cut = pfStrCutHFEdges )

# Add EtaMax
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
primaryVertexFilterLooseNDOF2 = primaryVertexFilter.clone()
primaryVertexFilterLooseNDOF2.cut = cms.string("!isFake && ndof > 2 && abs(z) <= 15 && position.Rho <= 2")
primaryVertexFilterLooseNDOF0 = primaryVertexFilter.clone()
primaryVertexFilterLooseNDOF0.cut = cms.string("!isFake && ndof > 0 && abs(z) <= 15 && position.Rho <= 2")

offlineSelectionNoVertex = cms.Sequence(filterScraping+
                                        HBHENoiseFilter+hcalNoiseFilter+
                                        multipleVertexVeto)
offlineSelection = cms.Sequence( primaryVertexFilter + offlineSelectionNoVertex )
offlineSelectionLooseNDOF0 = cms.Sequence( primaryVertexFilterLooseNDOF0 + offlineSelectionNoVertex)
offlineSelectionLooseNDOF2 = cms.Sequence( primaryVertexFilterLooseNDOF2 + offlineSelectionNoVertex)

eventSelection = cms.Sequence( offlineSelection )
eventSelectionNoVertex = cms.Sequence( offlineSelectionNoVertex )
eventSelectionZeroBias = cms.Sequence( hltZeroBias + offlineSelection )
eventSelectionZeroBiasNoVertex = cms.Sequence( hltZeroBias + offlineSelectionNoVertex )
eventSelectionMinBias = cms.Sequence( hltMinBias + offlineSelection )
eventSelectionMinBiasNoVertex = cms.Sequence( hltMinBias + offlineSelectionNoVertex )
eventSelectionMinBiasLooseNDOF0 = cms.Sequence( hltMinBias + offlineSelectionLooseNDOF0 )
eventSelectionMinBiasLooseNDOF2 = cms.Sequence( hltMinBias + offlineSelectionLooseNDOF2 )

eventSelectionMinBiasNoColl = cms.Sequence( hltMinBias + ~bptx + offlineSelection )
eventSelectionMinBiasNoCollNoVertex = cms.Sequence( hltMinBias + ~bptx + offlineSelectionNoVertex )
eventSelectionMinBiasNoCollLooseNDOF0 = cms.Sequence( hltMinBias + ~bptx + offlineSelectionLooseNDOF0 )
eventSelectionMinBiasNoCollLooseNDOF2 = cms.Sequence( hltMinBias + ~bptx + offlineSelectionLooseNDOF2 )

eventSelectionMinBiasEtaMaxFilter = cms.Sequence( eventSelectionMinBias + etaMaxFilter )
eventSelectionMinBiasEtaMaxFilterNoVertex = cms.Sequence( eventSelectionMinBiasNoVertex + etaMaxFilter )
eventSelectionMinBiasLooseNDOF0EtaMaxFilter = cms.Sequence( eventSelectionMinBiasLooseNDOF0 + etaMaxFilter )
eventSelectionMinBiasLooseNDOF2EtaMaxFilter = cms.Sequence( eventSelectionMinBiasLooseNDOF2 + etaMaxFilter )
eventSelectionMinBiasEtaMinFilter = cms.Sequence( eventSelectionMinBias + etaMinFilter )
eventSelectionMinBiasEtaMinFilterNoVertex = cms.Sequence( eventSelectionMinBiasNoVertex + etaMinFilter )
eventSelectionMinBiasLooseNDOF0EtaMinFilter = cms.Sequence( eventSelectionMinBiasLooseNDOF0 + etaMinFilter )
eventSelectionMinBiasLooseNDOF2EtaMinFilter = cms.Sequence( eventSelectionMinBiasLooseNDOF2 + etaMinFilter )
#-------------------------------------------
# Sequences
vertices = cms.Sequence(analysisVertices)
tracks = cms.Sequence(analysisTracks)
pfCandidates = cms.Sequence(pfCandidateNoiseThresholds* 
                            etaMaxPFCands+etaMinPFCands)
edmDump = cms.Sequence(hcalActivitySummary+hcalActivitySummaryScale090+hcalActivitySummaryScale092+
                       hcalActivitySummaryScale095+hcalActivitySummaryScale098+
                       hcalActivitySummaryScale102+hcalActivitySummaryScale105+
                       hcalActivitySummaryScale108+hcalActivitySummaryScale110+
                       edmNtupleEtaMax+edmNtupleEtaMin)
#-------------------------------------------
analysisSequences = cms.Sequence(vertices*tracks*pfCandidates*edmDump)
#-------------------------------------------
