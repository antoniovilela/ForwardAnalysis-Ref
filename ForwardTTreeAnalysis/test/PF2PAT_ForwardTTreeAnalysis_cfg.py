import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.writeEdmOutput = False
config.runOnMC = False
config.globalTagNameData = 'GR_R_42_V19::All' 
config.instLumiROOTFile='lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
globalTagNameMC = ''
#config.outputEdmFile = 'DijetsAnalysis.root'
config.outputTTreeFile = 'DijetsanalysisDATA42X_PATTTree_Apr21.root'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
#config.trackTagName = 'selectGoodTracks'
config.trackTagName = 'analysisTracks'
#config.generator = 'Pythia6'
config.varyAttributes = True
config.runOfflineOnly = True
config.runNoColl = False
config.runBPTX = False
config.runHCALFilter = False
#-----------------------------------------------------------------------------
config.UsePAT = True
#------------------------------------------------------------------------------
process = cms.Process("Analysis")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.threshold = 'INFO'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True),
SkipEvent = cms.untracked.vstring('ProductNotFound') )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        # 'file:/storage2/eliza/FWDAna/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/patTuple_PF2PAT_PromptReco2.root'
        #'file:/storage2/eliza/FWDAna/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/patTuple_PF2PAT_Apr21.root'
        'file:/storage2/eliza/samples_Apr21/PAT_JetMETTAU.root'
        
    )
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
'''
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
'''
###################################################################################
######################################
#Configuation using PF(OK)
process.load("ForwardAnalysis.ForwardTTreeAnalysis.ExclusiveDijetsAnalysis_cfi")
process.ExclusiveDijetsAnalysis.JetTag = "selectedPatJetsPFlow"
####################################
process.load("ForwardAnalysis.Utilities.tracksOutsideJets_cfi")
process.tracksOutsideJets.JetTag = "selectedPatJetsPFlow"

process.load("ForwardAnalysis.ForwardTTreeAnalysis.tracksTransverseRegion_cfi")
process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"


####################################################################################
# Analysis modules
#--------------------------------
from ForwardAnalysis.Utilities.countsAnalyzer_cfi import countsAnalyzer
#added by eliza
process.load('ForwardAnalysis.Utilities.lumiWeight_cfi')
process.lumiWeight.rootFileName = cms.string(config.instLumiROOTFile)
#end
 
 
process.load("ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsAnalysisSequences_cff")


#process.load("ForwardAnalysis.ForwardTTreeAnalysis.singleVertexFilter_cfi")

process.load('ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsTTreeAnalysis_cfi')
#process.exclusiveDijetsTTreeAnalysis.TriggerResultsTag = cms.InputTag("TriggerResults::HLT")
process.exclusiveDijetsTTreeAnalysis.EBeam = config.comEnergy/2.
process.exclusiveDijetsTTreeAnalysis.TrackTag = config.trackTagName
process.exclusiveDijetsTTreeAnalysis.ParticleFlowTag = "pfCandidateNoiseThresholds"
#process.exclusiveDijetsTTreeAnalysisPFNoiseThresholds = process.exclusiveDijetsTTreeAnalysis.clone(ParticleFlowTag = "pfCandidateNoiseThresholds")

#added by eliza
process.eventWeightSequence = cms.Sequence(process.lumiWeight) 
process.eventWeight_step = cms.Path(process.eventWeightSequence) 
#
process.analysis_reco_step = cms.Path(process.analysisSequences)
process.analysis_exclusiveDijetsAnalysis_step = cms.Path(process.eventSelectionHLT+
                                                         process.exclusiveDijetsTTreeAnalysis)
