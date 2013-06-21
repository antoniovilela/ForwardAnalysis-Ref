#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''

#
# Electron2010B
#

#'HLT_Mu0_L1MuOpen','HLT_Mu3','HLT_Mu5','HLT_DoubleMu0'

os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/May_2013/MuRunA2010/MuRunA2010_0.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_muon2010A_HLT_Mu0_L1MuOpen.root\" 1 0 1 30 15 1 0 0 1.0")

os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/May_2013/MuRunA2010/MuRunA2010_0.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_muon2010A_HLT_Mu3.root\" 1 1 1 30 15 1 0 0 1.0")

os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/May_2013/MuRunA2010/MuRunA2010_0.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_muon2010A_HLT_Mu5.root\" 1 2 1 30 15 1 0 0 1.0")

os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/May_2013/MuRunA2010/MuRunA2010_0.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_muon2010A_HLT_DoubleMu0.root\" 1 3 1 30 15 1 0 0 1.0")

#----------------------------------------------------------->>>


print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''
