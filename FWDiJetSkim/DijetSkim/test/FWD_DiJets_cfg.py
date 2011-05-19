import FWCore.ParameterSet.Config as cms

process = cms.Process("FWDDiJetsSkim")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
#number of Events to be skimmed.
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#replace fileNames  with the file you want to skim
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'file:/tmp/eliza/Multijet2011.root'

##   "/store/relval/CMSSW_3_2_6/RelValTTbar_Tauola_2M_PROD/GEN-SIM-RECO/MC_31X_V8-v1/0013/F46F8CA0-D09A-DE11-9DE6-001D09F251E0.root"
	    )
)

#load the EventContent and Skim cff/i files for EXOSingleJet sub-skim.
process.load('FWDiJetSkim.DijetSkim.FWD_DiJets_EventsContent_cfi')
process.load('FWDiJetSkim.DijetSkim.FWD_DiJetsSkim_cff')


#define output file name. 
process.forwardDiJetsOutputModule.fileName = cms.untracked.string('FWD_DiJets.root')


#user can select forwardDiJetsHLTQualitySeq or forwardDiJetsRecoQualitySeq
process.forwardDiJetsSkimPath=cms.Path(
                                    process.forwardDiJetsRecoQualitySeq

                                      )

process.endPath = cms.EndPath(process.forwardDiJetsOutputModule)
