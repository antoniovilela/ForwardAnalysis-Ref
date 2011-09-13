## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *
# ------------------------------------------------------
#  In addition you usually want to change the following
#  parameters:
## ------------------------------------------------------
#
process.GlobalTag.globaltag =  'GR_R_42_V19::All'

process.source.fileNames = [
   # 'file:/storage2/eliza/samples_test/MultiJetPromptReco2_v4.root'
     'file:/storage2/eliza/JetMay10RecoRun2011.root'
]

process.MessageLogger.cerr.FwkReport.reportEvery = 10

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.out.fileName = cms.untracked.string('patTuple_PF2PAT.root')

# load the PAT config
process.load("PhysicsTools.PatAlgos.patSequences_cff")

# Configure PAT to use PF2PAT instead of AOD sources
# this function will modify the PAT sequences. It is currently 
# not possible to run PF2PAT+PAT and standart PAT at the same time

###############################################################################
# Applying JEC using PF2PAT(Sandro F. de Souza in 09/05/11)
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuidePATTools#Jet_Tools
##############################################################################

from PhysicsTools.PatAlgos.tools.pfTools import *

postfix = "PFlow"
jetAlgo="AK5"
usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=False, postfix=postfix,jetCorrections=('AK5PFchs', ['L1FastJet','L2Relative','L3Absolute'])) 
#-----------------Customization----------------
process.pfPileUpPFlow.Enable = True
process.pfPileUpPFlow.checkClosestZVertex = False
process.pfPileUpPFlow.Vertices = cms.InputTag('goodOfflinePrimaryVertices')
process.pfPileUpPFlow.verbose = True
process.pfJetsPFlow.doAreaFastjet = True
process.pfJetsPFlow.doRhoFastjet = False

from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
    )

# Compute the mean pt per unit area (rho) from the PFCHS inputs
from RecoJets.Configuration.RecoPFJets_cff import kt6PFJets,ak5PFJets
process.kt6PFJetsPFlow = kt6PFJets.clone(
    src = cms.InputTag('pfNoElectron'+postfix),
    doAreaFastjet = cms.bool(True),
    doRhoFastjet = cms.bool(True)
    )
process.patJetCorrFactorsPFlow.rho = cms.InputTag("kt6PFJetsPFlow", "rho")

process.ak5PFJetsPileUp = ak5PFJets.clone( src = cms.InputTag('pfPileUp'+postfix) )

"""
process.pileUpN1PrimaryVertices = cms.EDFilter("RecoTauPileUpVertexSelector",
    filter = cms.bool(False),
    src = cms.InputTag('goodOfflinePrimaryVertices'),
    minTrackSumPt = cms.double(0.)
    #minTrackSumPt = cms.double(5.)
    )

process.pileUpN2PrimaryVertices = process.pileUpN1PrimaryVertices.clone(
    src = cms.InputTag('pileUpN1PrimaryVertices')
    )

process.pileUpN3PrimaryVertices = process.pileUpN1PrimaryVertices.clone(
    src = cms.InputTag('pileUpN2PrimaryVertices')
    )

process.pfPileUpN1 = process.pfPileUp.clone(
    PFCandidates = cms.InputTag('pfPileUp'+postfix),
    Vertices = cms.InputTag('pileUpN1PrimaryVertices'), 
    verbose = True
    )

process.pfPileUpExclN1 = process.pfNoPileUp.clone(
    name = cms.untracked.string('pileUpN1OnPFCandidates'),
    bottomCollection = cms.InputTag('pfPileUp'+postfix),
    topCollection = cms.InputTag('pfPileUpN1'),
    verbose = True
    )

process.pfPileUpN2 = process.pfPileUp.clone(
    PFCandidates = cms.InputTag('pfPileUpN1'),
    Vertices = cms.InputTag('pileUpN2PrimaryVertices'),
    verbose = True
    )

process.pfPileUpExclN2 = process.pfNoPileUp.clone(
    name = cms.untracked.string('pileUpN2OnPFCandidates'),
    bottomCollection = cms.InputTag('pfPileUpN1'),
    topCollection = cms.InputTag('pfPileUpN2'),
    verbose = True
    )

process.pfPileUpN3 = process.pfPileUp.clone(
    PFCandidates = cms.InputTag('pfPileUpN2'),
    Vertices = cms.InputTag('pileUpN3PrimaryVertices'),
    verbose = True
    )

process.pfPileUpExclN3 = process.pfNoPileUp.clone(
    name = cms.untracked.string('pileUpN3OnPFCandidates'),
    bottomCollection = cms.InputTag('pfPileUpN2'),
    topCollection = cms.InputTag('pfPileUpN3'),
    verbose = True
    )
"""

# Add modules to default sequence
getattr(process,"patPF2PATSequence"+postfix).replace(
    getattr(process,"pfNoElectron"+postfix),
    getattr(process,"pfNoElectron"+postfix)*process.kt6PFJetsPFlow
    )

"""
getattr(process,"patPF2PATSequence"+postfix).replace(
    getattr(process,"pfNoPileUp"+postfix),
    getattr(process,"pfNoPileUp"+postfix) * 
    process.pfPileUpN1 * process.pfPileUpN2 * process.pfPileUpN3 *
    process.pfPileUpExclN1 * process.pfPileUpExclN2 * process.pfPileUpExclN3
    )
"""

getattr(process,"patPF2PATSequence"+postfix).replace(
    getattr(process,"pfNoPileUp"+postfix),
    getattr(process,"pfNoPileUp"+postfix) * process.ak5PFJetsPileUp
    )

#----------------------------------------------
# Let it run
process.p = cms.Path(
    process.goodOfflinePrimaryVertices * 
#    process.pileUpN1PrimaryVertices * process.pileUpN2PrimaryVertices * process.pileUpN3PrimaryVertices *
#    process.patDefaultSequence  +
    getattr(process,"patPF2PATSequence"+postfix)
#    second PF2PAT
#    + getattr(process,"patPF2PATSequence"+postfix2)
)

# Add PF2PAT output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
#process.load("CommonTools.ParticleFlow.PF2PAT_EventContent_cff")
#process.out.outputCommands =  cms.untracked.vstring('drop *')
process.out.outputCommands = cms.untracked.vstring('drop *',
                                                   'keep recoPFCandidates_particleFlow_*_*',
                                                   *patEventContentNoCleaning ) 
# Adjust the event content
process.out.outputCommands += [
    #'keep *_selectedPat*_*_*',
    #'keep double_*PFlow*_*_PAT',
    'keep *_offlinePrimaryVertices*_*_*',
    'keep *_goodOfflinePrimaryVertices*_*_*',
    #'keep *_pileUpN*PrimaryVertices_*_*',
    #'keep *_pfPileUpExclN*_*_*',
    'keep *_pfPileUpPFlow*_*_*',
    'keep *_pfNoPileUpPFlow*_*_*',
    'keep *_ak5PFJetsPileUp_*_*',
    'keep *_ak5PFJets_*_*',
    'keep recoTracks_generalTracks_*_*',
    'keep HcalNoiseSummary_hcalnoise_*_*',
    'keep *_BeamHaloSummary_*_*', 
 #------- Trigger collections ------
    'keep edmTriggerResults_TriggerResults_*_*',
    'keep *_hltTriggerSummaryAOD_*_*',
    'keep L1GlobalTriggerObjectMapRecord_*_*_*',
    'keep L1GlobalTriggerReadoutRecord_*_*_*',
 #------- CASTOR rec hits ------
    'keep *_logErrorHarvester_*_*', 
    'keep *_castorreco_*_*',
 #------- Calo towers (just for now) ------ 
    'keep *_towerMaker_*_*' 
]

# top projections in PF2PAT:
getattr(process,"pfNoPileUp"+postfix).enable = True 
getattr(process,"pfNoMuon"+postfix).enable = True 
getattr(process,"pfNoElectron"+postfix).enable = True 
getattr(process,"pfNoTau"+postfix).enable = False 
getattr(process,"pfNoJet"+postfix).enable = True

# verbose flags for the PF2PAT modules
getattr(process,"pfNoJet"+postfix).verbose = False
