import FWCore.ParameterSet.Config as cms

#
# Program Settings
#

class config: pass
config.verbose = True
config.writeEdmOutput = False
config.outputTTreeFile = 'ZeroBiasTest.root'
config.runPATSequences = True
config.usePAT = True
config.globalTagNameData = 'GR_R_42_V23::All' 
config.instLumiROOTFile=''
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
config.NumberOfEvents = 10

#
# Define Triggers and Input Files
#

config.l1Paths = ('L1_ZeroBias','L1_BptxMinus_NotBptxPlus','L1_SingleJet30U')
config.hltPaths = ('HLT_ZeroBias', 'HLT_L1_BPTX_PlusOnly' ,'HLT_L1_BPTX_MinusOnly')   
config.inputFileName = '/storage1/eliza/samples_test/MinimumBias_ZeorBias.root'
config.runPUMC = False
config.runGen = False

#
# CMSSW Main Code
#

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.options = cms.untracked.PSet(
	wantSummary = cms.untracked.bool(False),
	SkipEvent = cms.untracked.vstring('ProductNotFound')
	)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 'file:%s' % config.inputFileName )
    #duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

#
# Output
#

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile))

#
# Detector Conditions and Scales
#

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

#
# Global Tag Input
#

process.GlobalTag.globaltag = config.globalTagNameData

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.load('RecoJets.Configuration.RecoJets_cff')
process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')

#
# PAT Sequences
#


if config.runPATSequences:
    from ForwardAnalysis.Skimming.addPATSequences import addPATSequences
    addPATSequences(process,False)
    process.patTrigger.addL1Algos = cms.bool( True )
    process.patJets.addTagInfos   = cms.bool( True )   

from ForwardAnalysis.Utilities.addCastorRecHitCorrector import addCastorRecHitCorrector
addCastorRecHitCorrector(process)


#
# Open Analysis Module
#


process.load("ForwardAnalysis.ExclusiveDijetsAnalysis.exclusiveDijetsAnalysisSequences_cff")
#process.pfCandidateNoiseThresholds.src = "pfNoPileUpPFlow"
process.pfCandidateNoiseThresholds.src = "particleFlow"
process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"

from ForwardAnalysis.ForwardTTreeAnalysis.DiffractiveAnalysis_cfi import DiffractiveAnalysis
from ForwardAnalysis.ForwardTTreeAnalysis.ExclusiveDijetsAnalysis_cfi import ExclusiveDijetsAnalysis
from ForwardAnalysis.ForwardTTreeAnalysis.PATTriggerInfo_cfi import PATTriggerInfo
from ForwardAnalysis.ForwardTTreeAnalysis.DijetsTriggerAnalysis_cfi import DijetsTriggerAnalysis  
#PATTriggerInfo.L1AlgoBitName =  config.l1Paths 
#PATTriggerInfo.HLTAlgoBitName = config.hltPaths 
PATTriggerInfo.runALLTriggerPath = True

#
# Module with EventInfo + DiffractiveAnalysis + ExclusiveDijets 
#

process.exclusiveDijetsAnalysisZeroBiasTTree = cms.EDAnalyzer("EventInfoDiffractiveExclusiveDijetsAnalysisTTree",
        EventInfo = cms.PSet(
                    RunOnData = cms.untracked.bool(True),
                    RunWithMCPU = cms.untracked.bool(False),
                    RunWithGen = cms.untracked.bool(False)
        ),
        DiffractiveAnalysis = DiffractiveAnalysis,
        ExclusiveDijetsAnalysis = ExclusiveDijetsAnalysis
        )

process.exclusiveDijetsHLTFilter.HLTPaths = ['HLT_ZeroBias', 'HLT_L1_BPTX_PlusOnly' ,'HLT_L1_BPTX_MinusOnly'] 

process.exclusiveDijetsAnalysisZeroBiasTTree.DiffractiveAnalysis.hltPath = ''
process.exclusiveDijetsAnalysisZeroBiasTTree.DiffractiveAnalysis.trackTag = 'analysisTracks'
process.exclusiveDijetsAnalysisZeroBiasTTree.DiffractiveAnalysis.vertexTag = "goodOfflinePrimaryVertices"
process.exclusiveDijetsAnalysisZeroBiasTTree.DiffractiveAnalysis.energyThresholdHF = 7.0
process.exclusiveDijetsAnalysisZeroBiasTTree.DiffractiveAnalysis.particleFlowTag = "pfCandidateNoiseThresholds"
process.exclusiveDijetsAnalysisZeroBiasTTree.DiffractiveAnalysis.jetTag = "selectedPatJetsPFlow"

process.exclusiveDijetsAnalysisZeroBiasTTree.ExclusiveDijetsAnalysis.hltPaths = ['HLT_ZeroBias', 'HLT_L1_BPTX_PlusOnly' ,'HLT_L1_BPTX_MinusOnly']
process.exclusiveDijetsAnalysisZeroBiasTTree.ExclusiveDijetsAnalysis.TrackTag = 'analysisTracks'
process.exclusiveDijetsAnalysisZeroBiasTTree.ExclusiveDijetsAnalysis.VertexTag = "goodOfflinePrimaryVertices"
process.exclusiveDijetsAnalysisZeroBiasTTree.ExclusiveDijetsAnalysis.ParticleFlowTag = "pfCandidateNoiseThresholds"
process.exclusiveDijetsAnalysisZeroBiasTTree.ExclusiveDijetsAnalysis.JetTag = "selectedPatJetsPFlow"
process.exclusiveDijetsAnalysisZeroBiasTTree.ExclusiveDijetsAnalysis.JetNonCorrTag = "ak5PFJets"

process.exclusiveDijetsAnalysisZeroBiasTTree.DiffractiveAnalysis.accessMCInfo = False 
process.exclusiveDijetsAnalysisZeroBiasTTree.ExclusiveDijetsAnalysis.AccessMCInfo = False 


process.analysis_reco_step = cms.Path(process.analysisSequences)
process.castor_step = cms.Path(process.castorSequence)


#
# Run Path 
#

process.exclusiveDijetsAnalysisZeroBiasTTreeAfterCuts = process.exclusiveDijetsAnalysisZeroBiasTTree.clone()

process.analysis_diffractiveExclusiveDijetsAnalysisPATTriggerInfoTTree_step = cms.Path(
    process.exclusiveDijetsHLTFilter +
    process.exclusiveDijetsAnalysisZeroBiasTTree +
    process.eventSelection + 
    process.exclusiveDijetsAnalysisZeroBiasTTreeAfterCuts )

