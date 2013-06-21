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


os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/January_2013/PATTupleElectron2010B_p1.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_electron2010B_p1_HLT_Ele17_SW_L1R.root\" 1 0 1 30 15 1 0 0 1.0")

os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/January_2013/PATTupleElectron2010B_p2.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_electron2010B_p2_HLT_Ele17_SW_L1R.root\" 1 0 1 30 15 1 0 0 1.0")

os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/January_2013/PATTupleElectron2010B_p3.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_electron2010B_p3_HLT_Ele17_SW_L1R.root\" 1 0 1 30 15 1 0 0 1.0")

os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/January_2013/PATTupleElectron2010B_p1.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_electron2010B_p1_HLT_Ele12_SW_TightEleId_L1R.root\" 1 1 1 30 15 1 0 0 1.0")

os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/January_2013/PATTupleElectron2010B_p2.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_electron2010B_p2_HLT_Ele22_SW_TighterEleId_L1R.root\" 1 1 1 30 15 1 0 0 1.0")

os.system("./DiffZ \"/storage1/dmf/Samples/PattuplesHF7/January_2013/PATTupleElectron2010B_p3.root\" \"diffractiveZAnalysisTTree/ProcessedTree\" \"histo_electron2010B_p3_HLT_Ele22_SW_TighterEleId_L1R.root\" 1 1 1 30 15 1 0 0 1.0")


#----------------------------------------------------------->>>


print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''
