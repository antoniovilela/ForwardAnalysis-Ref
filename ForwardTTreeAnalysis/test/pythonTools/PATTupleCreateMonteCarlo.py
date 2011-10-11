#!/usr/bin/env python
import os
import random
import time

def crabCall(uiworkingdir):
    print ''
    print '##########################################'
    print 'Creating job...'
    print '##########################################'
    print ''
    os.system('crab -create')
    print ''
    print '##########################################'
    print 'Submiting job...'
    print '##########################################'
    os.system('crab -submit -c '+ uiworkingdir)

def cmsRunCall():
    print ''
    print '##########################################'
    print 'Starting CMSSW...'
    print '##########################################'
    print ''
    os.system('cmsRun pset.py')
    print ''

def ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs):
    pset_py = """

import FWCore.ParameterSet.Config as cms

# Settings
class config: pass
config.verbose = True
config.writeEdmOutput = False
config.runPATSequences = True
config.runOnMC = True
config.usePAT = True
config.globalTagNameData = 'GR_R_42_V19::All' 
config.instLumiROOTFile='@@LUMIXS@@'
config.globalTagNameMC = 'START42_V14A::All'
config.comEnergy = 7000.0
config.trackAnalyzerName = 'trackHistoAnalyzer'
config.trackTagName = 'analysisTracks'
config.outputTTreeFile = '@@OUTPUT@@'
config.data2011 = False

if config.runOnMC:
    config.inputFileName = '/storage2/eliza/samples_test/QCD_Pt-15to30_TuneZ2_7TeV_pythia6AODSIMS_3.root'#MC
else:
    config.inputFileName = '/storage2/antoniov/data1/MultiJet_Run2010B_Apr21ReReco-v1_AOD/MultiJet_Run2010B_Apr21ReReco-v1_AOD_7EA7B611-7371-E011-B164-002354EF3BDB.root'#Data
   #  config.inputFileName = '/storage2/eliza/samples_test/MultiJetPromptReco_v4.root'#data 2011
process = cms.Process("Analysis")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

if config.runPATSequences:
    from ForwardAnalysis.Skimming.addPATSequences import addPATSequences
    addPATSequences(process,config.runOnMC)

if config.runOnMC:
    process.patTrigger.addL1Algos = cms.bool( False )
    process.patJets.addTagInfos   = cms.bool( False )
else:
    process.patTrigger.addL1Algos = cms.bool( True )
    process.patJets.addTagInfos   = cms.bool( True )   


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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(300) )

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

if config.runOnMC:
    process.exclusiveDijetsHLTFilter.HLTPaths = [@@TRIGGERSFILTER@@]

else:
    process.exclusiveDijetsHLTFilter.HLTPaths = [@@TRIGGERSFILTER@@] 

if config.data2011: process.exclusiveDijetsHLTFilter.HLTPaths = [@@TRIGGERSFILTER@@] 


process.pfCandidateNoiseThresholds.src = "pfNoPileUpPFlow"
process.tracksTransverseRegion.JetTag = "selectedPatJetsPFlow"

#process.load("ForwardAnalysis.ForwardTTreeAnalysis.singleVertexFilter_cfi")

process.load('ForwardAnalysis.ForwardTTreeAnalysis.exclusiveDijetsTTreeAnalysis_cfi')
castorTagName = "castorRecHitCorrector"
if config.runOnMC: castorTagName = "castorreco"

# Diffractive analysis
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.triggerResultsTag = cms.InputTag("TriggerResults::HLT")
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.hltPath = 'HLT_L1Tech_BSC_minBias_OR'
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.comEnergy = config.comEnergy
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.trackTag = config.trackTagName
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.vertexTag = "goodOfflinePrimaryVertices"
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.particleFlowTag = "pfCandidateNoiseThresholds"
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.jetTag = "selectedPatJetsPFlow"
process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.castorRecHitTag = castorTagName

if config.runOnMC:
    process.exclusiveDijetsTTreeAnalysis.diffractiveAnalysis.accessMCInfo = True

# Exclusive dijets analysis
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.EBeam = config.comEnergy/2.
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.TrackTag = config.trackTagName
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.VertexTag = "goodOfflinePrimaryVertices"
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.ParticleFlowTag = "pfCandidateNoiseThresholds"
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.JetTag = "selectedPatJetsPFlow"
process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.JetNonCorrTag = "ak5PFJets"

process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.TriggerResultsTag = cms.InputTag("TriggerResults::HLT")

if config.runOnMC:
    process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.AccessMCInfo = True
    process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.hltPaths = cms.vstring(@@TRIGGERSTTREE@@)
else: process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.hltPaths = cms.vstring(@@TRIGGERSTTREE@@)
if config.data2011: process.exclusiveDijetsTTreeAnalysis.exclusiveDijetsAnalysis.hltPaths = cms.vstring(@@TRIGGERSTTREE@@)

# PAT Trigger

#L1 Trigger
process.exclusiveDijetsTTreeAnalysis.patInfo.L1AlgoBitName = cms.vstring('L1_ZeroBias','L1_BptxMinus_NotBptxPlus','L1_SingleJet30U')
#HLT
process.exclusiveDijetsTTreeAnalysis.patInfo.HLTAlgoBitName = cms.vstring('HLT_ExclDiJet30U_HFOR_v1','HLT_DiJetAve100U_v1')

#added by eliza
if not config.runOnMC:
    process.eventWeightSequence = cms.Sequence(process.lumiWeight) 
    process.eventWeight_step = cms.Path(process.eventWeightSequence) 

if not config.runOnMC: process.castor_step = cms.Path(process.castorSequence)

if config.runOnMC:
   process.gen_step = cms.Path(process.genChargedParticles+
                               process.genProtonDissociative*process.edmNtupleMxGen+
                               process.genStableParticles*
                               process.etaMaxGen+process.etaMinGen*
                               process.edmNtupleEtaMaxGen+process.edmNtupleEtaMinGen)

process.analysis_reco_step = cms.Path(process.analysisSequences)
process.analysis_exclusiveDijetsAnalysis_step = cms.Path(process.eventSelectionHLT+
                                                         process.exclusiveDijetsTTreeAnalysis)
"""

    crab_cfg = """
[CRAB]
jobtype = cmssw
scheduler = glite
use_server =  1
#server_name = cern_vocms22 2.7.3- glite

[CMSSW]
total_number_of_events = @@NEVENTS@@
events_per_job = @@EJOB@@
datasetpath=@@DATASET@@
pset=pset.py
output_file=@@OUTPUT@@

[USER]
ui_working_dir=@@UIWORKINGDIR@@
return_data = 1
copy_data = 0
publish_data=0
email = dmf@cern.ch

#ui_working_dir=@@UIWORKINGDIR@@
#return_data = 0
#copy_data = 1
#publish_data=0
#email = dmf@cern.ch
#check_user_remote_dir = 1
#storage_element = se-dcache.hepgrid.uerj.br 
#storage_path = /srm/managerv2?SFN=/pnfs/hepgrid.uerj.br/data/cms/store/user/dmf
#storage_port = 8443
#user_remote_dir = @@USEREMOTE@@

[GRID]
proxy_server = myproxy.cern.ch
@@USESEWHITELIST@@se_white_list = @@SEWHITELIST@@
se_black_list = T3_US_COLORADO
virtual_organization = cms

"""
    print ''
    print '##########################################'
    print 'Removing old stuff...'
    print '##########################################'
    os.system('rm pset.py')
    os.system('rm crab.cfg')
    print ''
    print '##########################################'
    print 'Creating PSet file...'
    print '##########################################'

    pset_py = pset_py.replace('@@TRIGGERSFILTER@@',triggersfilter,-1)
    pset_py = pset_py.replace('@@TRIGGERSTTREE@@',triggersttree,-1)
    pset_py = pset_py.replace('@@OUTPUT@@',output,-1)
    pset_py = pset_py.replace('@@LUMIXS@@',lumixs,-1)

    pset_pyFile = open('pset.py','a+')
    pset_pyFile.write(pset_py)
    pset_pyFile.close()

    print ''
    print '##########################################'
    print 'Creating CRAB Config file...'
    print '##########################################'

    crab_cfg = crab_cfg.replace('@@DATASET@@',dataset,-1)
    crab_cfg = crab_cfg.replace('@@OUTPUT@@',output,-1)
    crab_cfg = crab_cfg.replace('@@UIWORKINGDIR@@',uiworkingdir,-1)
    crab_cfg = crab_cfg.replace('@@SEWHITELIST@@',sewhitelist,-1)
    crab_cfg = crab_cfg.replace('@@USEREMOTE@@',userremotedir,-1)
    crab_cfg = crab_cfg.replace('@@MAIL@@',mail,-1)
    crab_cfg = crab_cfg.replace('@@EJOB@@',ejob,-1)
    crab_cfg = crab_cfg.replace('@@NEVENTS@@',nevent,-1)
    #use or not se_white_list
    if (sewhitelist == ''):
        crab_cfg = crab_cfg.replace('@@USESEWHITELIST@@','#',-1)
    else:
        crab_cfg = crab_cfg.replace('@@USESEWHITELIST@@','',-1)
    crab_cfgFile = open('crab.cfg','a+')
    crab_cfgFile.write(crab_cfg)
    crab_cfgFile.close()

    ##crab or cmsRun
    crabCall(uiworkingdir)

    #cmsRunCall()
    #clear
    print ''
    print '##########################################'
    print 'Backuping crab.cfg and pset.py...'
    print '##########################################'
    os.system('mkdir -p '+uiworkingdir+'/crabCallerBackup')
    os.system('cp crab.cfg '+uiworkingdir+'/crabCallerBackup/.')
    os.system('cp pset.py '+uiworkingdir+'/crabCallerBackup/.')
    print ''
    print 'Backups saved: '+uiworkingdir+'/crabCallerBackup/'
    print ''
    print ''
    print '##########################################'
    print 'Cleaning...'
    print '##########################################'
    os.system('rm pset.py')
    os.system('rm crab.cfg')


print ''
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print 'Starting submission...'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'

##############
# MC Samples
##############

#
# TEST SAMPLE
#

dataset = '/QCD_Pt-120to170_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_TestMC'
userremotedir = '/crab_TestMC'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='10'
ejob = '100'
sewhitelist = ''
output = 'TEST.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
#ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#

dataset = '/QCD_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-15to3000-Z2'
userremotedir = '/crab_QCD_Pt-15to3000-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='5000000'
ejob = '20000'
sewhitelist = ''
output = 'QCD_Pt-15to3000-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-0to5_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM 
#

dataset = '/QCD_Pt-0to5_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-0to5-Z2'
userremotedir = '/crab_QCD_Pt-0to5-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='1000000'
ejob = '10000'
sewhitelist = ''
output = 'QCD_Pt-0to5-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-5to15_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#

dataset = '/QCD_Pt-5to15_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-5to15-Z2'
userremotedir = '/crab_QCD_Pt-5to15-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='1000000'
ejob = '10000'
sewhitelist = ''
output = 'QCD_Pt-5to15-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-15to30_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#

dataset = '/QCD_Pt-15to30_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-15to30-Z2'
userremotedir = '/crab_QCD_Pt-15to30-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='1000000'
ejob = '1000'
sewhitelist = ''
output = 'QCD_Pt-15to30-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-30to50_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#

dataset = '/QCD_Pt-30to50_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-30to50-Z2'
userremotedir = '/crab_QCD_Pt-30to50-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='1000000'
ejob = '10000'
sewhitelist = ''
output = 'QCD_Pt-30to50-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-50to80_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#

dataset = '/QCD_Pt-50to80_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-50to80-Z2'
userremotedir = '/crab_QCD_Pt-50to80-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='1000000'
ejob = '10000'
sewhitelist = ''
output = 'QCD_Pt-50to80-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-80to120_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#

dataset = '/QCD_Pt-80to120_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-80to120-Z2'
userremotedir = '/crab_QCD_Pt-80to120-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='1000000'
ejob = '10000'
sewhitelist = ''
output = 'QCD_Pt-80to120-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-120to170_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#

dataset = '/QCD_Pt-120to170_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-120to170-Z2'
userremotedir = '/crab_QCD_Pt-120to170-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='1000000'
ejob = '10000'
sewhitelist = ''
output = 'QCD_Pt-120to170-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-170to300_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#

dataset = '/QCD_Pt-170to300_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-170to300-Z2'
userremotedir = '/crab_QCD_Pt-170to300-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='1000000'
ejob = '10000'
sewhitelist = ''
output = 'QCD_Pt-170to300-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-300to470_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#

dataset = '/QCD_Pt-300to470_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-300to470-Z2'
userremotedir = '/crab_QCD_Pt-300to470-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='1000000'
ejob = '10000'
sewhitelist = ''
output = 'QCD_Pt-300to470-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-470to600_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#

dataset = '/QCD_Pt-470to600_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-470to600-Z2'
userremotedir = '/crab_QCD_Pt-470to600-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='1000000'
ejob = '10000'
sewhitelist = ''
output = 'QCD_Pt-470to600-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)

#
# Sample: /QCD_Pt-600to800_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM
#

dataset = '/QCD_Pt-600to800_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
uiworkingdir = 'crab_QCD_Pt-600to800-Z2'
userremotedir = '/crab_QCD_Pt-600to800-Z2'
lumixs = 'lumibylsXing_Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON.root'
nevent ='1000000'
ejob = '10000'
sewhitelist = ''
output = 'QCD_Pt-600to800-Z2.root'
triggersfilter = "'HLT_Jet30_v*','HLT_Jet60_v*'"
triggersttree = "'HLT_Jet30_v*','HLT_Jet60_v*'"
mail='dmf@cern.ch'
ConfigHandler(dataset,uiworkingdir,userremotedir,sewhitelist,output,triggersfilter,triggersttree,mail,nevent,ejob,lumixs)



print ''
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print 'Finishing submission...'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'
print '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@'



