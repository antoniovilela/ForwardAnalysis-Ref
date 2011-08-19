import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
#process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
exclusiveDijetsHLTFilter = copy.deepcopy(hltHighLevel)
#exclusiveDijetsHLTFilter.HLTPaths = ['HLT_DiJetAve30']
exclusiveDijetsHLTFilter.HLTPaths =['HLT_PhysicsDeclared']




'''
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.hltJets = copy.deepcopy(hltHighLevel)
process.hltJets.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")
process.hltJets.HLTPaths = ['HLT_Jet15U','HLT_Jet30U','HLT_Jet50U']
'''
