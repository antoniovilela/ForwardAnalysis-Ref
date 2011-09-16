## import skeleton process
from PhysicsTools.PatAlgos.patTemplate_cfg import *

process.GlobalTag.globaltag = ''
process.source.fileNames = ['file:']
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

# switch on PAT trigger
# included by SFonseca
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTrigger
#switchOnTrigger( process,triggerProducer = 'patTrigger', triggerEventProducer = 'patTriggerEvent', sequence = 'patPF2PATSequence',hltProcess = 'HLT', outputModule = 'out')
switchOnTrigger( process ,sequence ='patPF2PATSequence'+postfix)
process.patTrigger.addL1Algos = cms.bool( True )
switchOnTrigger( process ,sequence = 'patPF2PATSequence'+postfix) # to fix event content
#-------------------------------------------------------------------------------------------------------------

# Add modules to default sequence
getattr(process,"patPF2PATSequence"+postfix).replace(
    getattr(process,"pfNoElectron"+postfix),
    getattr(process,"pfNoElectron"+postfix)*process.kt6PFJetsPFlow
    )

getattr(process,"patPF2PATSequence"+postfix).replace(
    getattr(process,"pfNoPileUp"+postfix),
    getattr(process,"pfNoPileUp"+postfix) * process.ak5PFJetsPileUp
    )

#----------------------------------------------
# Let it run
process.pat_step = cms.Path(
    process.goodOfflinePrimaryVertices * 
    #process.patDefaultSequence  +
    getattr(process,"patPF2PATSequence"+postfix)
)

## Ref:http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/TopQuarkAnalysis/TopPairBSM/test/ttbsm_cfg.py?hideattic=0&revision=1.1.2.4&view=markup
# included by SFonseca
# Add PatTrigger output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent

# Add PF2PAT output to the created file
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
process.out.outputCommands += cms.untracked.vstring('drop *',
                                                   'keep recoPFCandidates_particleFlow_*_*',
                                                   *patEventContentNoCleaning) 

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
    'keep *_towerMaker_*_*',
 #---------------PatTrigger----------------
    'keep patTriggerObjects_patTrigger*_*_*',
    'keep patTriggerFilters_patTrigger*_*_*',
    'keep patTriggerPaths_patTrigger*_*_*',
    'keep patTriggerEvent_patTriggerEvent*_*_*'
]

# top projections in PF2PAT:
getattr(process,"pfNoPileUp"+postfix).enable = True 
getattr(process,"pfNoMuon"+postfix).enable = True 
getattr(process,"pfNoElectron"+postfix).enable = True 
getattr(process,"pfNoTau"+postfix).enable = False 
getattr(process,"pfNoJet"+postfix).enable = True

# verbose flags for the PF2PAT modules
getattr(process,"pfNoJet"+postfix).verbose = False
