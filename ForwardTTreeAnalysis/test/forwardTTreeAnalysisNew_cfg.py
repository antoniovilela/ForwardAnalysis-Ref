import FWCore.ParameterSet.Config as cms

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.options = cms.untracked.PSet(
	wantSummary = cms.untracked.bool(True),
	SkipEvent = cms.untracked.vstring('ProductNotFound')
	)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 'file:%s' % '/storage2/eliza/JetMay10RecoRun2011.root' )
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('diffractiveAnalysisPATInfoTTree.root'))

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_42_V19::All'

from ForwardAnalysis.Skimming.addPATSequences import addPATSequences
addPATSequences(process,False)
process.patTrigger.addL1Algos = cms.bool( True )
process.patJets.addTagInfos   = cms.bool( True )   

from ForwardAnalysis.ForwardTTreeAnalysis.addCastorRecHitCorrector import addCastorRecHitCorrector
addCastorRecHitCorrector(process)

process.load("ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsAnalysisSequences_cff")

from ForwardAnalysis.ForwardTTreeAnalysis.DiffractiveAnalysis_cfi import DiffractiveAnalysis
from ForwardAnalysis.ForwardTTreeAnalysis.ExclusiveDijetsAnalysis_cfi import ExclusiveDijetsAnalysis
from ForwardAnalysis.ForwardTTreeAnalysis.PATInfo_cfi import PATInfo
PATInfo.L1AlgoBitName = ['L1_SingleJet36','L1_SingleJet36_FwdVeto']
PATInfo.HLTAlgoBitName = ['HLT_ExclDiJet60_HFAND_v*','HLT_ExclDiJet60_HFOR_v*','HLT_Jet60_v*']
process.patInfoTTree = cms.EDAnalyzer("PATInfoTTree",
	PATInfo = PATInfo
	)
process.diffractiveAnalysisTTree = cms.EDAnalyzer("DiffractiveAnalysisTTree",
	DiffractiveAnalysis = DiffractiveAnalysis
	)
process.diffractiveAnalysisPATInfoTTree = cms.EDAnalyzer("DiffractiveAnalysisPATInfoTTree",
	DiffractiveAnalysis = DiffractiveAnalysis,
	PATInfo = PATInfo
	)
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree = cms.EDAnalyzer("DiffractiveExclusiveDijetsAnalysisPATInfoTTree",
	DiffractiveAnalysis = DiffractiveAnalysis,
        ExclusiveDijetsAnalysis = ExclusiveDijetsAnalysis,
	PATInfo = PATInfo
	)
process.diffractiveAnalysisTTree.DiffractiveAnalysis.hltPath = ''
process.diffractiveAnalysisPATInfoTTree.DiffractiveAnalysis.hltPath = ''

process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.DiffractiveAnalysis.hltPath = ''
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.DiffractiveAnalysis.trackTag = 'analysisTracks'
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.DiffractiveAnalysis.vertexTag = "goodOfflinePrimaryVertices"
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.DiffractiveAnalysis.particleFlowTag = "pfCandidateNoiseThresholds"
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.DiffractiveAnalysis.jetTag = "selectedPatJetsPFlow"

process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.hltPaths = cms.vstring('HLT_ExclDiJet60_HFAND_v*','HLT_ExclDiJet60_HFOR_v*','HLT_Jet60_v*')
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.TrackTag = 'analysisTracks'
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.VertexTag = "goodOfflinePrimaryVertices"
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.ParticleFlowTag = "pfCandidateNoiseThresholds"
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.JetTag = "selectedPatJetsPFlow"
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.JetNonCorrTag = "ak5PFJets"

#if config.runOnMC:
#   process.gen_step = cms.Path(process.genChargedParticles+
#                               process.genProtonDissociative*process.edmNtupleMxGen+
#                               process.genStableParticles*
#                               process.etaMaxGen+process.etaMinGen*
#                               process.edmNtupleEtaMaxGen+process.edmNtupleEtaMinGen)

process.analysis_reco_step = cms.Path(process.analysisSequences)
process.castor_step = cms.Path(process.castorSequence)
#process.analysis_patInfoTTree_step = cms.Path(process.patInfoTTree)
#process.analysis_diffractiveAnalysisTTree_step = cms.Path(process.diffractiveAnalysisTTree)
#process.analysis_diffractiveAnalysisPATInfoTTree_step = cms.Path(process.diffractiveAnalysisPATInfoTTree)
process.analysis_diffractiveExclusiveDijetsAnalysisPATInfoTTree_step = cms.Path(process.diffractiveExclusiveDijetsAnalysisPATInfoTTree)
