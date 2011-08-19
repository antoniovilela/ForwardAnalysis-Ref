import FWCore.ParameterSet.Config as cms
##---------------------------------------
from L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskAlgoTrigConfig_cff import *
from L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff import *
es_prefer_l1GtTriggerMaskAlgoTrig = cms.ESPrefer("L1GtTriggerMaskAlgoTrigTrivialProducer","l1GtTriggerMaskAlgoTrig")
es_prefer_l1GtTriggerMaskTechTrig = cms.ESPrefer("L1GtTriggerMaskTechTrigTrivialProducer","l1GtTriggerMaskTechTrig")
##--------------------------------------

from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import *
hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
#bptx = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('0'))
#bscAnd = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('40 OR 41'))
beamHaloVeto = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)'))

#l1CollBscAnd = cms.Sequence(bptx+bscAnd+beamHaloVeto)
#l1CollBscNoBptx = cms.Sequence(bscAnd+beamHaloVeto)
l1CollBscAnd = cms.Sequence(beamHaloVeto)
l1CollBscNoBptx = cms.Sequence(beamHaloVeto)


###--------
from ForwardAnalysis.ForwardTTreeAnalysis.primaryVertexFilter_cfi import *
##---------------------
from ForwardAnalysis.ForwardTTreeAnalysis.filterScraping_cfi import *
##-------------
from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import *
##-------------
## Jet Trigger Filter
######################

from ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsHLTPaths_cfi import *

hltBscJetFilter = exclusiveDijetsHLTFilter.clone(HLTPaths = ['HLT_Jet60_v*']) 

ak5PFL1L2L3 = cms.ESSource(
    'JetCorrectionServiceChain',
    correctors = cms.vstring('ak5PFL1Offset','ak5PFL2Relative','ak5PFL3Absolute')
    )

ak5PFJetsL2L3   = cms.EDProducer('PFJetCorrectionProducer',
    src         = cms.InputTag('ak5PFJets'),
    correctors  = cms.vstring('ak5PFL1L2L3')
    )
JetCorrectorSequence = cms.Sequence(ak5PFJetsL2L3)
#-----------------------------

goodJets = cms.EDFilter("CandViewSelector",
  src = cms.InputTag("ak5PFJets"),
  cut = cms.string("pt > 0.")
#  # ptMin = cms.double(20)
)
#------------------------------
jetFilter = cms.EDFilter("CandViewCountFilter",
   src = cms.InputTag("goodJets"),
   minNumber = cms.uint32(2)
)
jetFilterSequence = cms.Sequence(goodJets*jetFilter)

#------------------
from ForwardAnalysis.ForwardTTreeAnalysis.edmDumpAnalysis_cfi import *
#--------------------------
'''
from ForwardAnalysis.ForwardTTreeAnalysis.leadingJets_cfi import *
#leadingJets.src = "L2L3CorJetSC7PF"
leadingJets.src = "ak5PFJets"
'''
#------------SDW-----------------------
#from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectGoodTracks_cfi import *
#selectGoodTracks.cut = "pt > 0.5 & numberOfValidHits > 7 & d0 <= 3.5"
from PhysicsTools.RecoAlgos.recoTrackSelector_cfi import *
recoTrackSelector.src = "generalTracks"
recoTrackSelector.minRapidity = -2.5
recoTrackSelector.maxRapidity = 2.5
recoTrackSelector.ptMin = 0.5
recoTrackSelector.quality = ["highPurity"]
selectGoodTracks = recoTrackSelector
#from ForwardAnalysis.ForwardTTreeAnalysis.analysisTracks_cfi import *
from Utilities.AnalysisTools.analysisTracks_cfi import *
##-----------SDW
#from MyAnalysis.MyAnalysis.selectTracksAssociatedToPV_cfi import selectTracksAssociatedToPV as tracksAssociatedToPV
##tracksAssociatedToPV.src = "selectGoodTracks"
#tracksAssociatedToPV.src = "analysisTracks"
#tracksAssociatedToPV.MaxDistanceFromVertex = 1.0
#-------------------------------
from ForwardAnalysis.ForwardTTreeAnalysis.selectTracksAssociatedToPV_cfi import *
##tracksAssociatedToPV.src = "selectGoodTracks"
selectTracksAssociatedToPV.src = "analysisTracks"
selectTracksAssociatedToPV.MaxDistanceFromVertex = 1.0
##-----------

from ForwardAnalysis.ForwardTTreeAnalysis.tracksOutsideJets_cfi import *
#tracksOutsideJets.src = "selectTracksAssociatedToPV" 
#tracksOutsideJets.JetTag = "leadingJets"
#tracksOutsideJets.JetConeSize = 0.5
tracksOutsideJets = tracksOutsideJets.clone(src = "analysisTracks",JetTag = "ak5PFJets",JetConeSize = 0.5)

##----------------------------------------------------------------
from ForwardAnalysis.ForwardTTreeAnalysis.tracksTransverseRegion_cfi import *
tracksTransverseRegion.src = "selectTracksAssociatedToPV"
tracksTransverseRegion.JetTag = "ak5PFJets"

##-----------SDW----------------------------------------------------
from ForwardAnalysis.ForwardTTreeAnalysis.trackMultiplicity_cfi import * 
trackMultiplicity.TracksTag = "analysisTracks"
trackMultiplicityAssociatedToPV = trackMultiplicity.clone(TracksTag = "selectTracksAssociatedToPV")
#trackMultiplicityOutsideJets = trackMultiplicity.clone(TracksTag = "tracksOutsideJets")
trackMultiplicityTransverseRegion = trackMultiplicity.clone(TracksTag = "tracksTransverseRegion")
#trackMultiplicityOutsideFJets = trackMultiplicity.clone(TracksTag = "tracksOutsideFJets")
#trackMultiplicityBetweenJets = trackMultiplicity.clone(TracksTag = "tracksBetweenJets")

##------------------------------------------------------------

#pfCandidateNoiseThresholds.cut = PFCandidateNoiseStringCut(pfThresholds).cut()
# Add EtaMax
from ForwardAnalysis.Utilities.pfCandidateSelector_cfi import pfCandidateSelector as pfCandidateNoiseThresholds
from ForwardAnalysis.Utilities.ExcludeHFEdgesStringCut import ExcludeHFEdgesStringCut #add
from ForwardAnalysis.Utilities.PFCandidateNoiseStringCut import PFCandidateNoiseStringCut
# Change thresholds here if needed
#from ForwardAnalysis.Utilities.pfThresholds_cfi import pfThresholds
from ForwardAnalysis.ForwardTTreeAnalysis.pfThresholds_cfi import pfThresholds

pfStrCut1 = ExcludeHFEdgesStringCut().cut() #add
pfStrCut2 = PFCandidateNoiseStringCut(pfThresholds).cut() #add
pfStrCut = '%s & %s' % (pfStrCut1,pfStrCut2) #add
pfCandidateNoiseThresholds.cut = pfStrCut #add

#from ForwardAnalysis.ForwardTTreeAnalysis.pfThresholds_cfi import pfThresholds
pfCandidateNoiseThresholds.cut = PFCandidateNoiseStringCut(pfThresholds).cut()
from ForwardAnalysis.Utilities.etaMaxCandViewSelector_cfi import etaMaxCandViewSelector as etaMaxPFCands
from ForwardAnalysis.Utilities.etaMinCandViewSelector_cfi import etaMinCandViewSelector as etaMinPFCands
etaMaxPFCands.src = "pfCandidateNoiseThresholds"
etaMinPFCands.src = "pfCandidateNoiseThresholds"

from Utilities.AnalysisSequences.genChargedParticles_cfi import genChargedParticles
from Utilities.AnalysisSequences.genStableParticles_cfi import genStableParticles
genStableParticles.cut = 'status = 1 & ( ( pdgId != 2212 ) | ( pdgId == 2212 & abs(pz) < %f ) )' % (0.75*3500.0)
genProtonDissociative = genStableParticles.clone( cut = 'pdgId == 9902210' )
etaMaxGen = etaMaxPFCands.clone(src = "genStableParticles")
etaMinGen = etaMinPFCands.clone(src = "genStableParticles")

from Utilities.AnalysisSequences.edmNtupleCandView_cfi import edmNtupleCandView
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

from Utilities.AnalysisSequences.etaMaxSelector_cfi import etaMaxSelector as etaMaxFilter
etaMaxFilter.src = "etaMaxPFCands"
from Utilities.AnalysisSequences.etaMinSelector_cfi import etaMinSelector as etaMinFilter
etaMinFilter.src = "etaMinPFCands"

etaMaxGenFilter = etaMaxFilter.clone(src = "etaMaxGen")
etaMinGenFilter = etaMinFilter.clone(src = "etaMinGen")

'''

from ForwardAnalysis.Utilities.pfCandidateSelector_cfi import pfCandidateSelector as pfCandidateNoiseThresholds
from ForwardAnalysis.Utilities.PFCandidateNoiseStringCut import PFCandidateNoiseStringCut
# Change thresholds here if needed
from ForwardAnalysis.Utilities.pfThresholds_cfi import pfThresholds
pfCandidateNoiseThresholds.cut = PFCandidateNoiseStringCut(pfThresholds).cut()
from ForwardAnalysis.Utilities.etaMaxCandViewSelector_cfi import etaMaxCandViewSelector as etaMaxPFCands
from ForwardAnalysis.Utilities.etaMinCandViewSelector_cfi import etaMinCandViewSelector as etaMinPFCands
etaMaxPFCands.src = "pfCandidateNoiseThresholds"
etaMinPFCands.src = "pfCandidateNoiseThresholds"

from Utilities.AnalysisSequences.genChargedParticles_cfi import genChargedParticles
from Utilities.AnalysisSequences.genStableParticles_cfi import genStableParticles
genStableParticles.cut = 'status = 1 & ( ( pdgId != 2212 ) | ( pdgId == 2212 & abs(pz) < %f ) )' % (0.75*3500.0)
#genStableParticles.cut = 'status = 1 & ( pdgId != 9902210 )'
etaMaxGen = etaMaxPFCands.clone(src = "genStableParticles")
etaMinGen = etaMinPFCands.clone(src = "genStableParticles")

from Utilities.AnalysisSequences.edmNtupleCandView_cfi import edmNtupleCandView
edmNtupleEtaMax = edmNtupleCandView.clone(src = "etaMaxPFCands")
edmNtupleEtaMax.variables = cms.VPSet( cms.PSet( tag = cms.untracked.string("etaMax"),
                                                 quantity = cms.untracked.string("eta") ) )
edmNtupleEtaMin = edmNtupleCandView.clone(src = "etaMinPFCands")
edmNtupleEtaMin.variables = cms.VPSet( cms.PSet( tag = cms.untracked.string("etaMin"),
                                                 quantity = cms.untracked.string("eta") ) )

edmNtupleEtaMaxGen = edmNtupleEtaMax.clone(src = "etaMaxGen")
edmNtupleEtaMinGen = edmNtupleEtaMin.clone(src = "etaMinGen")
'''
from ForwardAnalysis.Utilities.caloActivityFilter_cfi import caloActivityFilter
caloVetoHFPlus = caloActivityFilter.clone(NTowersMaxHFPlus=0)
caloVetoHFMinus = caloActivityFilter.clone(NTowersMaxHFMinus=0)
caloActivityFilter.EnergyThresholdHB = 1.5
caloActivityFilter.EnergyThresholdHE = 2.0
caloActivityFilter.EnergyThresholdHF = 4.0
#hcalVetoHBPlusAndMinus = caloActivityFilter.clone(NTowersMaxHBPlus = 0, NTowersMaxHBMinus = 0)
#hcalVetoHBHEPlusAndMinus = caloActivityFilter.clone(NTowersMaxHBPlus = 0, NTowersMaxHEPlus = 0, NTowersMaxHBMinus = 0, NTowersMaxHEMinus = 0)
#hcalVetoSumEMaxHBPlusAndMinus4 = caloActivityFilter.clone(SumEMaxHBPlus = 4.0, SumEMaxHBMinus = 4.0)
#hcalVetoSumEMaxHBPlusAndMinus8 = caloActivityFilter.clone(SumEMaxHBPlus = 8.0, SumEMaxHBMinus = 8.0)
#hcalVetoSumEMaxHBPlusAndMinus12 = caloActivityFilter.clone(SumEMaxHBPlus = 12.0, SumEMaxHBMinus = 12.0)
#hcalVetoSumEMaxHBPlusAndMinus16 = caloActivityFilter.clone(SumEMaxHBPlus = 16.0, SumEMaxHBMinus = 16.0)

#eventSelectionBscANDHBVetoPlusAndMinus = cms.Sequence(eventSelectionBscAND+hcalVetoHBPlusAndMinus)
#eventSelectionBscANDHBHEVetoPlusAndMinus = cms.Sequence(eventSelectionBscAND+hcalVetoHBHEPlusAndMinus)
#eventSelectionBscANDSumEMaxHBPlusAndMinus4 = cms.Sequence(eventSelectionBscAND+hcalVetoSumEMaxHBPlusAndMinus4)
#eventSelectionBscANDSumEMaxHBPlusAndMinus8 = cms.Sequence(eventSelectionBscAND+hcalVetoSumEMaxHBPlusAndMinus8)
#eventSelectionBscANDSumEMaxHBPlusAndMinus12 = cms.Sequence(eventSelectionBscAND+hcalVetoSumEMaxHBPlusAndMinus12)
#eventSelectionBscANDSumEMaxHBPlusAndMinus16 = cms.Sequence(eventSelectionBscAND+hcalVetoSumEMaxHBPlusAndMinus16)
#------------------------------------------------------------------
from ForwardAnalysis.ForwardTTreeAnalysis.hfTower_cfi import *
from ForwardAnalysis.ForwardTTreeAnalysis.xiTower_cfi import *
from ForwardAnalysis.ForwardTTreeAnalysis.xiFromCaloTowers_cfi import * 
xiTower.UseMETInfo = False
xiFromCaloTowers.ParticlesTag = cms.InputTag("towerMaker")
xiFromCaloTowers.UseMETInfo = False
##-----------------------------------------------------------------
from Utilities.AnalysisTools.hcalActivitySummary_cfi import *
hcalActivitySummary.DiscardFlaggedTowers = False
hcalActivitySummaryScale090 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.90,EnergyScaleFactorHE = 0.90,EnergyScaleFactorHF = 0.90)
hcalActivitySummaryScale092 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.92,EnergyScaleFactorHE = 0.92,EnergyScaleFactorHF = 0.92)
hcalActivitySummaryScale095 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.95,EnergyScaleFactorHE = 0.95,EnergyScaleFactorHF = 0.95)
hcalActivitySummaryScale098 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.98,EnergyScaleFactorHE = 0.98,EnergyScaleFactorHF = 0.98)
hcalActivitySummaryScale102 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.02,EnergyScaleFactorHE = 1.02,EnergyScaleFactorHF = 1.02)
hcalActivitySummaryScale105 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.05,EnergyScaleFactorHE = 1.05,EnergyScaleFactorHF = 1.05)
hcalActivitySummaryScale108 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.08,EnergyScaleFactorHE = 1.08,EnergyScaleFactorHF = 1.08)
hcalActivitySummaryScale110 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.10,EnergyScaleFactorHE = 1.10,EnergyScaleFactorHF = 1.10)
####################################################

###hltBscAND = cms.Sequence(l1CollBscAnd + hltBscMNFilter)
hltBscAND = cms.Sequence(l1CollBscAnd +hltBscJetFilter)
hltBscNoBptx = cms.Sequence(hltBscJetFilter)

##--------------------------------------------------------------
#preSelection = cms.Sequence()
#offlineSelection = cms.Sequence(primaryVertexFilter+filterScraping+HBHENoiseFilter)
offlineSelection = cms.Sequence(primaryVertexFilter+filterScraping+HBHENoiseFilter+JetCorrectorSequence+jetFilterSequence)
eventSelection = cms.Sequence(offlineSelection)
#***********
#eventSelectionBPTX = cms.Sequence(bptx+offlineSelection)
#***********
eventSelectionBscAND = cms.Sequence(hltBscAND+offlineSelection)
#***********
eventSelectionBscNoBptx = cms.Sequence(hltBscNoBptx+offlineSelection)
#***********
eventSelectionHLT = cms.Sequence(hltBscJetFilter+offlineSelection)
#***********
#eventSelectionHLTAND = cms.Sequence(hltDiJetFilterAND+offlineSelection)
#***********
#eventSelectionHLTOR = cms.Sequence(hltDiJetFilterOR+offlineSelection)
#-------------------------------------------------------

from Utilities.AnalysisTools.hcalActivityFilter_cfi import hcalActivityFilter
hcalActivityFilter.EnergyThresholdHB = 1.5
hcalActivityFilter.EnergyThresholdHE = 2.0
hcalActivityFilter.EnergyThresholdHF = 4.0
hcalVetoHBPlusAndMinus = hcalActivityFilter.clone(NTowersMaxHBPlus = 0, NTowersMaxHBMinus = 0)
hcalVetoHBHEPlusAndMinus = hcalActivityFilter.clone(NTowersMaxHBPlus = 0, NTowersMaxHEPlus = 0, NTowersMaxHBMinus = 0, NTowersMaxHEMinus = 0)
hcalVetoSumEMaxHBPlusAndMinus4 = hcalActivityFilter.clone(SumEMaxHBPlus = 4.0, SumEMaxHBMinus = 4.0)
hcalVetoSumEMaxHBPlusAndMinus8 = hcalActivityFilter.clone(SumEMaxHBPlus = 8.0, SumEMaxHBMinus = 8.0)
hcalVetoSumEMaxHBPlusAndMinus12 = hcalActivityFilter.clone(SumEMaxHBPlus = 12.0, SumEMaxHBMinus = 12.0)
hcalVetoSumEMaxHBPlusAndMinus16 = hcalActivityFilter.clone(SumEMaxHBPlus = 16.0, SumEMaxHBMinus = 16.0)

eventSelectionBscANDHBVetoPlusAndMinus = cms.Sequence(eventSelectionBscAND+hcalVetoHBPlusAndMinus)
eventSelectionBscANDHBHEVetoPlusAndMinus = cms.Sequence(eventSelectionBscAND+hcalVetoHBHEPlusAndMinus)
eventSelectionBscANDSumEMaxHBPlusAndMinus4 = cms.Sequence(eventSelectionBscAND+hcalVetoSumEMaxHBPlusAndMinus4)
eventSelectionBscANDSumEMaxHBPlusAndMinus8 = cms.Sequence(eventSelectionBscAND+hcalVetoSumEMaxHBPlusAndMinus8)
eventSelectionBscANDSumEMaxHBPlusAndMinus12 = cms.Sequence(eventSelectionBscAND+hcalVetoSumEMaxHBPlusAndMinus12)
eventSelectionBscANDSumEMaxHBPlusAndMinus16 = cms.Sequence(eventSelectionBscAND+hcalVetoSumEMaxHBPlusAndMinus16)

##--------------------------------------------
#analysisBeforeSelection = cms.EDAnalyzer("ExclusiveDijetsFilter",
 #   JetTag = cms.InputTag("ak5PFJets")
#)
#goodeta = cms.EDAnalyzer("GoodEtaSelection")
#analysisAfterSelection = analysisBeforeSelection.clone()

#hlt = cms.Sequence(exclusiveDijetsHLTFilter)
#jets = cms.Sequence(leadingJets)

#-------------------------------------------
#tracks = cms.Sequence(analysisTracks*tracksAssociatedToPV*tracksOutsideJets)
tracks = cms.Sequence(analysisTracks*
                      selectTracksAssociatedToPV*
                    #  tracksOutsideJets*
                      tracksTransverseRegion)
#+                     tracksTransverseRegion) 
###------------------------------
pfCandidates = cms.Sequence(pfCandidateNoiseThresholds*etaMaxPFCands+etaMinPFCands)

#edmDump = cms.Sequence(trackMultiplicity+trackMultiplicityAssociatedToPV+trackMultiplicityOutsideJets
         #             )
#####----------------------------
edmDump = cms.Sequence(trackMultiplicity+
                       trackMultiplicityAssociatedToPV+
                       #trackMultiplicityOutsideJets+
                       trackMultiplicityTransverseRegion+
                      # caloActivityFilter+
                       hcalActivitySummary+hcalActivitySummaryScale090+hcalActivitySummaryScale092+
                       hcalActivitySummaryScale095+hcalActivitySummaryScale098+
                       hcalActivitySummaryScale102+hcalActivitySummaryScale105+
                       hcalActivitySummaryScale108+hcalActivitySummaryScale110+
                       hfTower+xiTower+xiFromCaloTowers+
                       edmNtupleEtaMax+edmNtupleEtaMin)
#analysisBefore = cms.Sequence(analysisBeforeSelection)
#analysisAfter = cms.Sequence(hltBscJetFilter+analysisAfterSelection)

