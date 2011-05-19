import FWCore.ParameterSet.Config as cms

import HLTrigger.HLTfilters.triggerResultsFilter_cfi as hlt
DiJetAveSkim_Trigger = hlt.triggerResultsFilter.clone()
DiJetAveSkim_Trigger.triggerConditions = cms.vstring(
                           'HLT_Jet*',
                           'HLT_ExclDiJet*',
#                           'HLT_ExclDiJet60_HFOR_v*',
#                           'HLT_DiJetAve15U_v*',
                           'HLT_DiJetAve*',
#                           'HLT_Jet60_v*',
                                                   )

DiJetAveSkim_Trigger.hltResults = cms.InputTag( "TriggerResults", "", "HLT" )
DiJetAveSkim_Trigger.l1tResults = cms.InputTag("")

#Define the Reco quality cut
forwardRecoJetFilter = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("ak5PFJets"),
    cut = cms.string("pt > 20.0"),
    filter = cms.bool(True)
)

#Define the Di-jets Reco selection
forwardRecoDiJetsFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("forwardRecoJetFilter"),
    minNumber = cms.uint32(2),
    filter = cms.bool(True)                                     
)


#Define group sequence, using HLT bits + either HLT/Reco quality cut. 
forwardDiJetsHLTQualitySeq = cms.Sequence(
   DiJetAveSkim_Trigger
)
forwardDiJetsRecoQualitySeq = cms.Sequence(
	DiJetAveSkim_Trigger + 
        forwardRecoJetFilter*forwardRecoDiJetsFilter
)
