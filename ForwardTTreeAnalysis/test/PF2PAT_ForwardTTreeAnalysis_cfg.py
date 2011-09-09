import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.writeEdmOutput = False
config.runOnMC = False
config.usePAT = True
config.globalTagNameData = 'GR_R_42_V19::All' 
config.instLumiROOTFile='lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
globalTagNameMC = ''
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
#config.generator = 'Pythia6'

config.inputFileName = '/storage2/antoniov/data1/PATTuple/patTuple_PF2PAT.root'
#config.outputEdmFile = 'DijetsAnalysis.root'
config.outputTTreeFile = 'exclusiveDijetsanalysis_PATTTree.root'

process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
SkipEvent = cms.untracked.vstring('ProductNotFound') )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( 'file:%s' % config.inputFileName )
    #duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

#-------------------------------------------------------------------------------
# Import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.options.wantSummary = True
#--------------------------------------------------------------------------------
if config.runOnMC: process.GlobalTag.globaltag = config.globalTagNameMC
else: process.GlobalTag.globaltag = config.globalTagNameData

#---------------------------------------------------------------------------------
##Jet Correction Service

process.ak5CaloL1Offset.useCondDB = False
process.ak5PFL1Offset.useCondDB = False
process.ak5JPTL1Offset.useCondDB = False

#---------------------------------------------------------------------------------
# Output
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(config.outputTTreeFile))

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
######################################
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
