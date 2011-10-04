import FWCore.ParameterSet.Config as cms



def addPATSequences(process,runMC):
    # Load the PAT config
    from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
    process.out = cms.OutputModule("PoolOutputModule",
	    fileName = cms.untracked.string('patTuple.root'),
	    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('pat_step') ),
	    outputCommands = cms.untracked.vstring('drop *', *patEventContent )
            )
    ###
    process.load('PhysicsTools.PatAlgos.patSequences_cff')

    from PhysicsTools.PatAlgos.tools.pfTools import usePF2PAT
    postfix = "PFlow"
    jetAlgo="AK5"
   # runOnMC = False
    usePF2PAT(process,runPF2PAT=True, jetAlgo=jetAlgo, runOnMC=runMC, postfix=postfix,jetCorrections=('AK5PFchs', ['L1FastJet','L2Relative','L3Absolute'])) 
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

    # Switch on PAT trigger
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

    # Add PatTrigger output to the created file
    from PhysicsTools.PatAlgos.patEventContent_cff import patTriggerEventContent
    # Add PF2PAT output to the created file
    from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
    patOutputCommands = cms.untracked.vstring('drop *',
	    'keep recoPFCandidates_particleFlow_*_*',
	    *patEventContentNoCleaning) 

    # Adjust the event content
    patOutputCommands += [
	#'keep *_selectedPat*_*_*',
	'keep *_genParticles*_*_*',
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
    process.out.outputCommands = patOutputCommands

    # top projections in PF2PAT:
    getattr(process,"pfNoPileUp"+postfix).enable = True 
    getattr(process,"pfNoMuon"+postfix).enable = True 
    getattr(process,"pfNoElectron"+postfix).enable = True 
    getattr(process,"pfNoTau"+postfix).enable = False 
    getattr(process,"pfNoJet"+postfix).enable = True

    # verbose flags for the PF2PAT modules
    getattr(process,"pfNoJet"+postfix).verbose = False
