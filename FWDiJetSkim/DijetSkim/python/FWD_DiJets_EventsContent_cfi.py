import FWCore.ParameterSet.Config as cms
from Configuration.EventContent.EventContent_cff import *


forwardDiJetsOutputModule = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring(),
    SelectEvents = cms.untracked.PSet(
       SelectEvents = cms.vstring("forwardDiJetsSkimPath") #the selector name must be same as the path name in EXOSingleJet_cfg.py in test directory.
      ),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('FWD_DiJets'), #name a name you like.
        dataTier = cms.untracked.string('FWDGroup')
    ),
    fileName = cms.untracked.string('forwardDiJetstest.root') # can be modified later in EXOSingleJet_cfg.py in  test directory. 
  )


#default output contentRECOSIMEventContent
forwardDiJetsOutputModule.outputCommands.extend(RECOSIMEventContent.outputCommands)

#add specific content you need. 
SpecifiedEvenetContent=cms.PSet(
    outputCommands = cms.untracked.vstring(
      #Filter
      "keep *_forwardHLTDiJetsFilter_*_*",
      "keep *_forwardRecoJetFilter_*_*",
      "keep *_forwardRecoDiJetsFilter_*_*",
       # Jets
        'keep *_kt6CaloJets_*_*',
        'keep *_ak5CaloJets_*_*',
        'keep *_ak7CaloJets_*_*',
        'keep *_sisCone5CaloJets_*_*',
        'keep *_sisCone7CaloJets_*_*', 
        'keep *_kt4PFJets_*_*',
        'keep *_kt6PFJets_*_*',
        'keep *_ak5PFJets_*_*',
        'keep *_ak7PFJets_*_*',
        'keep *_sisCone5PFJets_*_*',
        'keep *_sisCone7PFJets_*_*',
         # RECO
        'keep *_EventAuxilary_*_*',
        'keep edmTriggerResults_TriggerResults_*_*',
        'keep *_hltTriggerSummaryAOD_*_*',
        'keep L1GlobalTriggerObjectMapRecord_*_*_*',
        'keep L1GlobalTriggerReadoutRecord_*_*_*',
        'keep *_hcalnoise_*_*',
        'keep *_CSCHaloData_*_*',
        'keep *_EcalHaloData_*_*',
        'keep *_HcalHaloData_*_*',
        'keep *_GlobalHaloData_*_*',
        'keep *_BeamHaloSummary_*_*',
        'keep recoTracks_generalTracks_*_*',
        'keep recoTracks_ctfPixelLess_*_*',
        'keep recoTracks_pixelTracks_*_*',
        'keep *_pixelVertices_*_*',
        'keep *_offlinePrimaryVertices_*_*',
        'keep *_offlinePrimaryVerticesWithBS_*_*',
        'keep *_particleFlow_*_*',
        'keep *_caloTowers_*_*',
        'keep *_towerMaker_*_*',
        'keep *_siPixelRecHits_*_*',
        'keep *_hfrecoReflagged_*_*',
        'keep *_hbherecoReflagged_*_*',
        'keep GenRunInfoProduct_*_*_*',
        'keep GenEventInfoProduct_*_*_*',
        'keep edmHepMCProduct_*_*_*',
       #------- CaloJet collections ------
        'keep recoCaloJets_kt4CaloJets_*_*',
        'keep recoCaloJets_kt6CaloJets_*_*',
        'keep recoCaloJets_ak5CaloJets_*_*',
        'keep recoCaloJets_ak7CaloJets_*_*',
        'keep recoCaloJets_iterativeCone5CaloJets_*_*',
        #------- CaloJet ID ---------------
        'keep *_kt4JetID_*_*',
        'keep *_kt6JetID_*_*',
        'keep *_ak5JetID_*_*',
        'keep *_ak7JetID_*_*',
        'keep *_ic5JetID_*_*',
        #------- PFJet collections ------ 
        'keep recoPFJets_kt4PFJets_*_*',
        'keep recoPFJets_kt6PFJets_*_*',
        'keep recoPFJets_ak5PFJets_*_*',
        'keep recoPFJets_ak7PFJets_*_*',
        'keep recoPFJets_iterativeCone5PFJets_*_*',
        #------- JPTJet collections ------
        'keep *_JetPlusTrackZSPCorJetAntiKt5_*_*',
       
        #------- Trigger collections ------
        'keep edmTriggerResults_TriggerResults_*_*',
        'keep *_hltTriggerSummaryAOD_*_*',
        'keep L1GlobalTriggerObjectMapRecord_*_*_*',
        'keep L1GlobalTriggerReadoutRecord_*_*_*',
         #------- MET collections ----------
        'keep *_metHO_*_*',
        'keep *_metNoHF_*_*',
        'keep *_metNoHFHO_*_*',
        'keep *_met_*_*'
      
      )
    )
forwardDiJetsOutputModule.outputCommands.extend(SpecifiedEvenetContent.outputCommands)

