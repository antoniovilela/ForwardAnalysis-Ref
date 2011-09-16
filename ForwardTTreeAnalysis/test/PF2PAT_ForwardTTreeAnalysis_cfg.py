import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.writeEdmOutput = False
config.runPATSequences = True
config.runOnMC = False
config.usePAT = True
config.globalTagNameData = 'GR_R_42_V19::All' 
config.instLumiROOTFile='lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
globalTagNameMC = ''
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
#config.generator = 'Pythia6'

config.inputFileName = '/storage2/eliza/JetMay10RecoRun2011.root'
config.outputTTreeFile = 'exclusiveDijetsanalysis_PATTTree.root'

process = cms.Process("Analysis")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#process.load("CondCore.DBCommon.CondDBCommon_cfi")
if config.runPATSequences:
    from ForwardAnalysis.Skimming.addPATSequences import addPATSequences
    addPATSequences(process)  

process.load('FWCore.MessageService.MessageLogger_cfi')
if not config.verbose:
    process.MessageLogger.cerr.threshold = 'INFO'
else:
    process.MessageLogger.cerr.threshold = 'DEBUG'
    process.MessageLogger.debugModules = cms.untracked.vstring('exclusiveDijetsTTreeAnalysis')
    process.MessageLogger.destinations = cms.untracked.vstring('cerr')
    process.MessageLogger.categories.append('Analysis')
    process.MessageLogger.cerr.Analysis = cms.untracked.PSet(limit = cms.untracked.int32(-1))

process.options = cms.untracked.PSet( 
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 'file:%s' % config.inputFileName )
    #duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

# Output
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile))
#-------------------------------------------------------------------------------

if config.runOnMC: process.GlobalTag.globaltag = config.globalTagNameMC
else: process.GlobalTag.globaltag = config.globalTagNameData

#---------------------------------------------------------------------------------
###################################################################################
# CASTOR RecHit Corrector
if not config.runOnMC:
    process.castorRecHitCorrector = cms.EDProducer("RecHitCorrector",
        rechitLabel = cms.InputTag("castorreco","","RECO"),
        revertFactor = cms.double(1),
        doInterCalib = cms.bool(False)
    )

    process.load("CondCore.DBCommon.CondDBSetup_cfi")
    process.CastorDbProducer = cms.ESProducer("CastorDbProducer")
    process.es_castor_pool = cms.ESSource("PoolDBESSource",
        process.CondDBSetup,
        timetype = cms.string('runnumber'),
        connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierProd/CMS_COND_31X_HCAL'),
        authenticationMethod = cms.untracked.uint32(0),
        toGet = cms.VPSet(
            cms.PSet(
                record = cms.string('CastorChannelQualityRcd'),  
                tag = cms.string('CastorChannelQuality_v2.2_offline')
            )
        )
    )
    process.es_prefer_castor = cms.ESPrefer('PoolDBESSource','es_castor_pool')

    process.castorInvalidDataFilter = cms.EDFilter("CastorInvalidDataFilter")
    process.castorSequence = cms.Sequence(process.castorInvalidDataFilter+process.castorRecHitCorrector)

###################################################################################
# Analysis sequences
process.load("ForwardAnalysis.Utilities.tracksOutsideJets_cfi")
process.tracksOutsideJets.JetTag = "selectedPatJetsPFlow"
process.load("ForwardAnalysis.ForwardTTreeAnalysis.tracksTransverseRegion_cfi")
process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"

####################################################################################
# Analysis modules
#--------------------------------
from ForwardAnalysis.Utilities.countsAnalyzer_cfi import countsAnalyzer
#added by eliza
if not config.runOnMC:
    process.load('ForwardAnalysis.Utilities.lumiWeight_cfi')
    process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)
#end
 
process.load("ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsAnalysisSequences_cff")
process.pfCandidateNoiseThresholds.src = "pfNoPileUpPFlow"

#process.load("ForwardAnalysis.ForwardTTreeAnalysis.singleVertexFilter_cfi")

process.load('ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsTTreeAnalysis_cfi')
# Diffractive analysis
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.triggerResultsTag = cms.InputTag("TriggerResults::HLT")
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.comEnergy = config.comEnergy
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.trackTag = config.trackTagName
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.vertexTag = "goodOfflinePrimaryVertices"
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.particleFlowTag = "pfCandidateNoiseThresholds"
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.jetTag = "selectedPatJetsPFlow"
# Exclusice dijets analysis
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.TriggerResultsTag = cms.InputTag("TriggerResults::HLT")
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.hltPaths = cms.vstring('HLT_ExclDiJet60_HFAND_v*','HLT_ExclDiJet60_HFOR_v*','HLT_Jet60_v*')
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.EBeam = config.comEnergy/2.
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.TrackTag = config.trackTagName
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.VertexTag = "goodOfflinePrimaryVertices"
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.ParticleFlowTag = "pfCandidateNoiseThresholds"
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.JetTag = "selectedPatJetsPFlow"

#added by eliza
if not config.runOnMC:
    process.eventWeightSequence = cms.Sequence(process.lumiWeight) 
    process.eventWeight_step = cms.Path(process.eventWeightSequence) 

if not config.runOnMC: process.castor_step = cms.Path(process.castorSequence)

process.analysis_reco_step = cms.Path(process.analysisSequences)
process.analysis_exclusiveDijetsAnalysis_step = cms.Path(process.eventSelectionHLT+
                                                         process.exclusiveDijetsTTreeAnalysis)
