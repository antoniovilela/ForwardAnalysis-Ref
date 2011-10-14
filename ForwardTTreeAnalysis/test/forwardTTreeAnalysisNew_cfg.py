import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.writeEdmOutput = False
config.runOnMC = False
config.runPATSequences = True
config.usePAT = False
config.globalTagNameData = 'GR_R_42_V19::All' 
#config.instLumiROOTFile='/storage2/eliza/lumibyXing_Cert_160404-176023_7TeV_PromptReco_Collisions11_JSON.root'
config.globalTagNameMC = 'START42_V14A::All'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
##PAT INFO does not using Asterisk, yet.

if config.runOnMC:
    #config.l1Paths = ('ALL')
    config.l1Paths = ('L1_ZeroBias','L1_BptxMinus_NotBptxPlus','L1_SingleJet30U')
    config.hltPaths =('HLT_Jet30_v1','HLT_Jet30_v2','HLT_Jet30_v3','HLT_Jet30_v4','HLT_Jet30_v5','HLT_Jet30_v6')
else:
    config.l1Paths = ('L1_ZeroBias','L1_BptxMinus_NotBptxPlus','L1_SingleJet30U')
    config.hltPaths = ('HLT_ExclDiJet30U_HFOR_v1','HLT_DiJetAve100U_v1')


config.outputTTreeFile = 'forwardQCDTTreeAnalysis.root'

if config.runOnMC:
    config.inputFileName = '/storage2/eliza/samples_test/QCD_Pt-15to30_TuneZ2_7TeV_pythia6AODSIMS_3.root'# MC
else:
    #config.inputFileName = '/storage2/eliza/samples_test/MultiJetPromptReco_v4.root'#data 2011
    config.inputFileName = '/storage2/antoniov/data1/MultiJet_Run2010B_Apr21ReReco-v1_AOD/MultiJet_Run2010B_Apr21ReReco-v1_AOD_7EA7B611-7371-E011-B164-002354EF3BDB.root' 


process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.options = cms.untracked.PSet(
	wantSummary = cms.untracked.bool(True),
	SkipEvent = cms.untracked.vstring('ProductNotFound')
	)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 'file:%s' % config.inputFileName )
    #duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)


#---------------------------------------------------------------------------------
# Output
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile))



process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_42_V19::All'


if config.runPATSequences:
    from ForwardAnalysis.Skimming.addPATSequences import addPATSequences
    addPATSequences(process,config.runOnMC)

    if config.runOnMC:
	process.patTrigger.addL1Algos = cms.bool( False )
	process.patJets.addTagInfos   = cms.bool( False )
    else:
	process.patTrigger.addL1Algos = cms.bool( True )
	process.patJets.addTagInfos   = cms.bool( True )   

## if not config.runOnMC:
##     process.eventWeightSequence = cms.Sequence(process.lumiWeight) 
##     process.eventWeight_step = cms.Path(process.eventWeightSequence)

from ForwardAnalysis.ForwardTTreeAnalysis.addCastorRecHitCorrector import addCastorRecHitCorrector
addCastorRecHitCorrector(process)

process.load("ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsAnalysisSequences_cff")
#######################################################################################################################
# Analysis modules
#--------------------------------
## from ForwardAnalysis.Utilities.countsAnalyzer_cfi import countsAnalyzer
## if not config.runOnMC:
##     process.load('ForwardAnalysis.Utilities.lumiWeight_cfi')
##     process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)
############################################################################################################## 
process.load("ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsAnalysisSequences_cff")

if config.runOnMC:
    process.exclusiveDijetsHLTFilter.HLTPaths = config.hltPaths 
else:
    process.exclusiveDijetsHLTFilter.HLTPaths = config.hltPaths 

process.pfCandidateNoiseThresholds.src = "pfNoPileUpPFlow"
process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"

########################################################################

from ForwardAnalysis.ForwardTTreeAnalysis.DiffractiveAnalysis_cfi import DiffractiveAnalysis
from ForwardAnalysis.ForwardTTreeAnalysis.ExclusiveDijetsAnalysis_cfi import ExclusiveDijetsAnalysis
from ForwardAnalysis.ForwardTTreeAnalysis.PATInfo_cfi import PATInfo

PATInfo.L1AlgoBitName =  config.l1Paths 
PATInfo.HLTAlgoBitName = config.hltPaths 
PATInfo.runALLTriggerPath = True


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

process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.hltPaths = config.hltPaths 
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.TrackTag = 'analysisTracks'
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.VertexTag = "goodOfflinePrimaryVertices"
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.ParticleFlowTag = "pfCandidateNoiseThresholds"
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.JetTag = "selectedPatJetsPFlow"
process.diffractiveExclusiveDijetsAnalysisPATInfoTTree.ExclusiveDijetsAnalysis.JetNonCorrTag = "ak5PFJets"

if config.runOnMC:
  process.gen_step = cms.Path(process.genChargedParticles+
                              process.genProtonDissociative*process.edmNtupleMxGen+
                              process.genStableParticles*
                              process.etaMaxGen+process.etaMinGen*
                              process.edmNtupleEtaMaxGen+process.edmNtupleEtaMinGen)

process.analysis_reco_step = cms.Path(process.analysisSequences)
process.castor_step = cms.Path(process.castorSequence)
#process.analysis_patInfoTTree_step = cms.Path(process.patInfoTTree)
#process.analysis_diffractiveAnalysisTTree_step = cms.Path(process.diffractiveAnalysisTTree)
process.analysis_diffractiveAnalysisPATInfoTTree_step = cms.Path(process.diffractiveAnalysisPATInfoTTree)
#process.analysis_diffractiveExclusiveDijetsAnalysisPATInfoTTree_step = cms.Path(process.diffractiveExclusiveDijetsAnalysisPATInfoTTree)
