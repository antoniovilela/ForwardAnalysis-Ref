import FWCore.ParameterSet.Config as cms

from Utilities.AnalysisSequences.hltLevel1GTSeed_cff import *
#beamHaloVeto = hltLevel1GTSeed.clone(L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)'))

from Utilities.AnalysisSequences.hltFilter_cfi import *
hltJet15U = hltFilter.clone(HLTPaths = ['HLT_Jet15U*'])
hltJet30U = hltFilter.clone(HLTPaths = ['HLT_Jet30U*'])

############
from Utilities.AnalysisSequences.primaryVertexFilter_cfi import *

from Utilities.AnalysisSequences.filterScraping_cfi import *

from CommonTools.RecoAlgos.HBHENoiseFilter_cfi import *

from Utilities.AnalysisTools.hcalNoiseFilter_cfi import *

from Utilities.AnalysisTools.analysisTracks_cfi import *

from Utilities.AnalysisSequences.analysisVertices_cfi import *

from Utilities.AnalysisTools.multipleVertexFilter_cfi import *
multipleVertexFilter.src = 'primaryVertexFilter'
#multipleVertexFilter.src = 'analysisVertices'
multipleVertexVeto = cms.Sequence(~multipleVertexFilter)

from Utilities.AnalysisSequences.leadingJets_cfi import *
leadingJets.src = "ak5PFJets"
leadingJets.maxNumber = 1

goodJetsPt30 = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("leadingJets"),
    cut = cms.string('pt > 30.0 & abs(eta) < 2.5'),
    filter = cms.bool(True) 
)
goodJetsPt60 = goodJetsPt30.clone(cut = 'pt > 60.0 & abs(eta) < 2.5') 

############
#hltSeq = cms.Sequence(l1CollBscOr + hltBscMinBiasORBptxPlusORMinusFilter)

offlineSelection = cms.Sequence(primaryVertexFilter+filterScraping+
                                HBHENoiseFilter+hcalNoiseFilter+
                                multipleVertexVeto)
eventSelection = cms.Sequence(offlineSelection)
#eventSelectionBeamHaloVeto = cms.Sequence(beamHaloVeto+offlineSelection)
eventSelectionHLTJet15UPt30 = cms.Sequence(hltJet15U+offlineSelection+leadingJets*goodJetsPt30)
eventSelectionHLTJet30UPt60 = cms.Sequence(hltJet30U+offlineSelection+leadingJets*goodJetsPt60)

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
