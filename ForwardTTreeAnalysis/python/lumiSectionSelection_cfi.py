import FWCore.ParameterSet.Config as cms

lumiSectionSelection = cms.EDFilter("LumiSectionSelection",
      Runs = cms.vuint32(124009,124020,124022,124023,124024,124025,124027,124030,124120),
      LumiSectionMin = cms.vuint32(1,12,66,38,2,5,24,2,1),
      LumiSectionMax = cms.vuint32(68,94,179,9999,83,13,9999,9999,9999), 
      applyfilter = cms.untracked.bool(True)
)
