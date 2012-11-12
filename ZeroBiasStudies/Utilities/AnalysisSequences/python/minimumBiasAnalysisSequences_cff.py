import FWCore.ParameterSet.Config as cms

from Utilities.AnalysisSequences.hltLevel1GTSeed_cff import *
bptx = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('0'))
l1Tech4 = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('4'))
bscOr = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('34'))
bscAnd = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('40 OR 41'))
beamHaloVeto = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)'))

l1Coll = cms.Sequence(bptx+beamHaloVeto)
l1CollBscOr = cms.Sequence(bptx+bscOr+beamHaloVeto)
l1CollBscAnd = cms.Sequence(bptx+bscAnd+beamHaloVeto)
l1NoColl = cms.Sequence(~bptx+beamHaloVeto)
l1NoCollBscOr = cms.Sequence(~bptx+bscOr+beamHaloVeto)
l1NoBPTX = cms.Sequence(beamHaloVeto)
l1NoBPTXBscOr = cms.Sequence(bscOr+beamHaloVeto)

from Utilities.AnalysisSequences.hltFilter_cfi import *
hltBscMinBiasORBptxPlusORMinusFilter = hltFilter.clone(HLTPaths = ['HLT_L1_BscMinBiasOR_BptxPlusORMinus'])

############
from Utilities.AnalysisSequences.primaryVertexFilter_cfi import *

from Utilities.AnalysisSequences.filterScraping_cfi import *

from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import *

from Utilities.AnalysisTools.hcalNoiseFilter_cfi import *

from Utilities.AnalysisTools.analysisTracks_cfi import *

from Utilities.AnalysisSequences.analysisVertices_cfi import *

from Utilities.AnalysisTools.multipleVertexFilter_cfi import *
multipleVertexFilter.src = 'analysisVertices'
multipleVertexVeto = cms.Sequence(~multipleVertexFilter)

"""
#from PhysicsTools.RecoAlgos.recoTrackSelector_cfi import *
#recoTrackSelector.src = "generalTracks"
#recoTrackSelector.minRapidity = -2.4
#recoTrackSelector.maxRapidity = 2.4
#recoTrackSelector.ptMin = 0.5
#recoTrackSelector.quality = ["highPurity"]
#selectGoodTracks = recoTrackSelector
"""
"""
from Utilities.AnalysisSequences.leadingJets_cfi import *
leadingJets.src = ""
"""
"""
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.selectTracksAssociatedToPV_cfi import selectTracksAssociatedToPV as tracksAssociatedToPV
tracksAssociatedToPV.src = "analysisTracks"
tracksAssociatedToPV.MaxDistanceFromVertex = 1.0
"""
"""
from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.tracksOutsideJets_cfi import *
tracksOutsideJets.src = "analysisTracks" 
tracksOutsideJets.JetTag = "leadingJets"
tracksOutsideJets.JetConeSize = 0.5
"""
"""
from ExclusiveDijetsAnalysis.ExclusiveDijetsAnalysis.tracksTransverseRegion_cfi import *
tracksTransverseRegion.src = "analysisTracks"
tracksTransverseRegion.JetTag = "leadingJets"
"""
"""
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.trackMultiplicity_cfi import * 
trackMultiplicity.TracksTag = "analysisTracks"
trackMultiplicityAssociatedToPV = trackMultiplicity.clone(TracksTag = "tracksAssociatedToPV")
trackMultiplicityOutsideJets = trackMultiplicity.clone(TracksTag = "tracksOutsideJets")
trackMultiplicityTransverseRegion = trackMultiplicity.clone(TracksTag = "tracksTransverseRegion")
"""
"""
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiTower_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiFromCaloTowers_cfi import *
from DiffractiveForwardAnalysis.SingleDiffractiveWAnalysis.xiFromJets_cfi import *
xiTower.UseMETInfo = False
xiTower.comEnergy = 7000.0
xiFromCaloTowers.UseMETInfo = False
xiFromCaloTowers.comEnergy = 7000.0
xiFromJets.UseMETInfo = False
xiFromJets.comEnergy = 7000.0
"""

############
from Utilities.AnalysisTools.hcalActivitySummary_cfi import *
hcalActivitySummary.DiscardFlaggedTowers = False
hcalActivitySummary.NumberOfThresholds = 200
hcalActivitySummaryScale090 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.90,EnergyScaleFactorHE = 0.90,EnergyScaleFactorHF = 0.90,EnergyScaleFactorEB = 0.90,EnergyScaleFactorEE = 0.90)
hcalActivitySummaryScale092 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.92,EnergyScaleFactorHE = 0.92,EnergyScaleFactorHF = 0.92,EnergyScaleFactorEB = 0.92,EnergyScaleFactorEE = 0.92)
hcalActivitySummaryScale095 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.95,EnergyScaleFactorHE = 0.95,EnergyScaleFactorHF = 0.95,EnergyScaleFactorEB = 0.95,EnergyScaleFactorEE = 0.95)
hcalActivitySummaryScale098 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 0.98,EnergyScaleFactorHE = 0.98,EnergyScaleFactorHF = 0.98,EnergyScaleFactorEB = 0.98,EnergyScaleFactorEE = 0.98)
hcalActivitySummaryScale102 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.02,EnergyScaleFactorHE = 1.02,EnergyScaleFactorHF = 1.02,EnergyScaleFactorEB = 1.02,EnergyScaleFactorEE = 1.02)
hcalActivitySummaryScale105 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.05,EnergyScaleFactorHE = 1.05,EnergyScaleFactorHF = 1.05,EnergyScaleFactorEB = 1.05,EnergyScaleFactorEE = 1.05)
hcalActivitySummaryScale108 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.08,EnergyScaleFactorHE = 1.08,EnergyScaleFactorHF = 1.08,EnergyScaleFactorEB = 1.08,EnergyScaleFactorEE = 1.08)
hcalActivitySummaryScale110 = hcalActivitySummary.clone(ApplyEnergyScale = True,EnergyScaleFactorHB = 1.10,EnergyScaleFactorHE = 1.10,EnergyScaleFactorHF = 1.10,EnergyScaleFactorEB = 1.10,EnergyScaleFactorEE = 1.10)

############
############
hltBscMinBiasOR = cms.Sequence(l1CollBscOr + hltBscMinBiasORBptxPlusORMinusFilter)
hltBscMinBiasORNoColl = cms.Sequence(l1NoCollBscOr + hltBscMinBiasORBptxPlusORMinusFilter)
hltBscMinBiasORBPTXOR = cms.Sequence(l1NoBPTXBscOr + hltBscMinBiasORBptxPlusORMinusFilter)

offlineSelection = cms.Sequence(primaryVertexFilter+filterScraping+
                                HBHENoiseFilter+hcalNoiseFilter+
                                multipleVertexVeto)
eventSelection = cms.Sequence(offlineSelection)
eventSelectionBeamHaloVeto = cms.Sequence(beamHaloVeto+offlineSelection)

eventSelectionBPTX = cms.Sequence(bptx+offlineSelection)
eventSelectionL1Tech4 = cms.Sequence(l1Tech4+offlineSelection)
eventSelectionL1Tech4BeamHaloVeto = cms.Sequence(l1Tech4+beamHaloVeto+offlineSelection)
eventSelectionL1Tech4BscOr = cms.Sequence(l1Tech4+bscOr+beamHaloVeto+offlineSelection)
eventSelectionL1Tech4HLTBscMinBiasOR = cms.Sequence(l1Tech4+hltBscMinBiasORBptxPlusORMinusFilter+
                                                            bscOr+beamHaloVeto+offlineSelection)

eventSelectionBscMinBiasOR = cms.Sequence(hltBscMinBiasOR+offlineSelection)
eventSelectionBscMinBiasORNoColl = cms.Sequence(hltBscMinBiasORNoColl+offlineSelection)
eventSelectionBscMinBiasORBPTXOR = cms.Sequence(hltBscMinBiasORBPTXOR+offlineSelection) 

############
from ForwardAnalysis.Utilities.caloActivityFilter_cfi import *
caloActivityFilter.EnergyThresholdHB = 1.25
caloActivityFilter.EnergyThresholdHE = 1.9
caloActivityFilter.EnergyThresholdHF = 4.0
caloActivityFilter.EnergyThresholdEB = 0.6
caloActivityFilter.EnergyThresholdEE = 2.45
caloVetoHFPlus = caloActivityFilter.clone(NTowersMaxHFPlus = 0)
caloVetoHFMinus = caloActivityFilter.clone(NTowersMaxHFMinus = 0)
caloVetoHEHFPlus = caloActivityFilter.clone(NTowersMaxHEPlus = 0, NTowersMaxHFPlus = 0)
caloVetoHEHFMinus = caloActivityFilter.clone(NTowersMaxHEMinus = 0, NTowersMaxHFMinus = 0)
caloVetoEEHEHFPlus = caloActivityFilter.clone(NTowersMaxEEPlus = 0, NTowersMaxHEPlus = 0, NTowersMaxHFPlus = 0)
caloVetoEEHEHFMinus = caloActivityFilter.clone(NTowersMaxEEMinus = 0, NTowersMaxHEMinus = 0, NTowersMaxHFMinus = 0)
caloVetoHFPlusAndMinus = caloActivityFilter.clone(NTowersMaxHFPlus = 0, NTowersMaxHFMinus = 0)
caloVetoHEHFPlusAndMinus = caloActivityFilter.clone(NTowersMaxHEPlus = 0, NTowersMaxHFPlus = 0, NTowersMaxHEMinus = 0, NTowersMaxHFMinus = 0)
caloVetoEEHEHFPlusAndMinus = caloActivityFilter.clone(NTowersMaxEEPlus = 0, NTowersMaxHEPlus = 0, NTowersMaxHFPlus = 0, NTowersMaxEEMinus = 0, NTowersMaxHEMinus = 0, NTowersMaxHFMinus = 0)
caloVetoSumEMaxHFPlus4 = caloActivityFilter.clone(SumEMaxHFPlus = 4.0)
caloVetoSumEMaxHFPlus8 = caloActivityFilter.clone(SumEMaxHFPlus = 8.0)
caloVetoSumEMaxHFPlus12 = caloActivityFilter.clone(SumEMaxHFPlus = 12.0)
caloVetoSumEMaxHFPlus16 = caloActivityFilter.clone(SumEMaxHFPlus = 16.0)
caloVetoSumEMaxHFMinus4 = caloActivityFilter.clone(SumEMaxHFMinus = 4.0)
caloVetoSumEMaxHFMinus8 = caloActivityFilter.clone(SumEMaxHFMinus = 8.0)
caloVetoSumEMaxHFMinus12 = caloActivityFilter.clone(SumEMaxHFMinus = 12.0)
caloVetoSumEMaxHFMinus16 = caloActivityFilter.clone(SumEMaxHFMinus = 16.0)
caloVetoSumETMaxHFPlus05 = caloActivityFilter.clone(SumETMaxHFPlus = 0.5)
caloVetoSumETMaxHFPlus10 = caloActivityFilter.clone(SumETMaxHFPlus = 1.0)
caloVetoSumETMaxHFPlus15 = caloActivityFilter.clone(SumETMaxHFPlus = 1.5)
caloVetoSumETMaxHFPlus20 = caloActivityFilter.clone(SumETMaxHFPlus = 2.0)
caloVetoSumETMaxHFMinus05 = caloActivityFilter.clone(SumETMaxHFMinus = 0.5)
caloVetoSumETMaxHFMinus10 = caloActivityFilter.clone(SumETMaxHFMinus = 1.0)
caloVetoSumETMaxHFMinus15 = caloActivityFilter.clone(SumETMaxHFMinus = 1.5)
caloVetoSumETMaxHFMinus20 = caloActivityFilter.clone(SumETMaxHFMinus = 2.0)

############
eventSelectionBscMinBiasORHFVetoPlus = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoHFPlus)
eventSelectionBscMinBiasORHFVetoMinus = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoHFMinus)
eventSelectionBscMinBiasORHEHFVetoPlus = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoHEHFPlus)
eventSelectionBscMinBiasORHEHFVetoMinus = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoHEHFMinus)
eventSelectionBscMinBiasOREEHEHFVetoPlus = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoEEHEHFPlus)
eventSelectionBscMinBiasOREEHEHFVetoMinus = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoEEHEHFMinus)
eventSelectionBscMinBiasORHFPlusAndMinus = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoHFPlusAndMinus)
eventSelectionBscMinBiasORHEHFPlusAndMinus = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoHEHFPlusAndMinus)
eventSelectionBscMinBiasOREEHEHFPlusAndMinus = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoEEHEHFPlusAndMinus)
eventSelectionBscMinBiasORSumEMaxHFPlus4 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumEMaxHFPlus4)
eventSelectionBscMinBiasORSumEMaxHFPlus8 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumEMaxHFPlus8)
eventSelectionBscMinBiasORSumEMaxHFPlus12 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumEMaxHFPlus12)
eventSelectionBscMinBiasORSumEMaxHFPlus16 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumEMaxHFPlus16)
eventSelectionBscMinBiasORSumEMaxHFMinus4 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumEMaxHFMinus4)
eventSelectionBscMinBiasORSumEMaxHFMinus8 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumEMaxHFMinus8)
eventSelectionBscMinBiasORSumEMaxHFMinus12 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumEMaxHFMinus12)
eventSelectionBscMinBiasORSumEMaxHFMinus16 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumEMaxHFMinus16)
eventSelectionBscMinBiasORSumETMaxHFPlus05 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumETMaxHFPlus05)
eventSelectionBscMinBiasORSumETMaxHFPlus10 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumETMaxHFPlus10)
eventSelectionBscMinBiasORSumETMaxHFPlus15 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumETMaxHFPlus15)
eventSelectionBscMinBiasORSumETMaxHFPlus20 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumETMaxHFPlus20)
eventSelectionBscMinBiasORSumETMaxHFMinus05 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumETMaxHFMinus05)
eventSelectionBscMinBiasORSumETMaxHFMinus10 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumETMaxHFMinus10)
eventSelectionBscMinBiasORSumETMaxHFMinus15 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumETMaxHFMinus15)
eventSelectionBscMinBiasORSumETMaxHFMinus20 = cms.Sequence(eventSelectionBscMinBiasOR+caloVetoSumETMaxHFMinus20)

############
"""
jets = cms.Sequence(leadingJets)
tracks = cms.Sequence(selectGoodTracks+
                      analysisTracks* 
                      tracksAssociatedToPV*
                      tracksOutsideJets+
                      tracksTransverseRegion) 
edmDump = cms.Sequence(trackMultiplicity+
                       hcalActivitySummary+hcalActivitySummaryScale090+hcalActivitySummaryScale092+
                       hcalActivitySummaryScale095+hcalActivitySummaryScale098+
                       hcalActivitySummaryScale102+hcalActivitySummaryScale105+
                       hcalActivitySummaryScale108+hcalActivitySummaryScale110+
                       xiTower+xiFromCaloTowers+xiFromJets)
"""
vertices = cms.Sequence(analysisVertices)
tracks = cms.Sequence(analysisTracks)
edmDump = cms.Sequence(hcalActivitySummary+hcalActivitySummaryScale090+hcalActivitySummaryScale092+
                       hcalActivitySummaryScale095+hcalActivitySummaryScale098+
                       hcalActivitySummaryScale102+hcalActivitySummaryScale105+
                       hcalActivitySummaryScale108+hcalActivitySummaryScale110)
