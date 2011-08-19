import FWCore.ParameterSet.Config as cms

from ForwardAnalysis.ForwardTTreeAnalysis.MyDijetsAnalysis_cfi import MyDijetsAnalysis

exclusiveDijetsTTreeAnalysis = cms.EDAnalyzer("ExclusiveDijetsTTreeAnalyzer",
    MyDijetsAnalysis#,
    # Saves or not ROOT TTree
#    SaveROOTTree = cms.untracked.bool(True)
)
