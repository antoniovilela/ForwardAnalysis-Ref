#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''


os.system("./TriggerEffCom \"/storage1/eliza/PATTuples_Mar2013/data_Multijets/data_total/data_MultiJetRB2010_0.root\" \"histo_effTriggerMultijetsRunB_RefOr30U_And30U_castor.root\" \"exclusiveDijetsAnalysisTTree/ProcessedTree\" 3 4 1 1")

os.system("./TriggerEffCom \"/storage1/eliza/PATTuples_Mar2013/data_Multijets/data_total/data_MultiJetRB2010_0.root\" \"histo_effTriggerMultijetsRunB_RefDijetAve50U_Or30U_castor.root\" \"exclusiveDijetsAnalysisTTree/ProcessedTree\" 7 3 1 1")

#os.system("./TriggerEffCom \"/storage1/eliza/PATTuples_HF7_Nov2012/data_SinalPF7/data_new/merge/MultijetPF7_p1p2_new_0.root\" \"histo_effTriggerMultijetsRunB_RefOr30U_And30U.root\" \"exclusiveDijetsAnalysisTTree/ProcessedTree\" 3 4 4")

#os.system("./TriggerEffCom \"/storage1/eliza/PATTuples_HF7_Nov2012/data_SinalPF7/data_new/merge/MultijetPF7_p1p2_new_0.root\" \"histo_effTriggerMultijetsRunB_RefDijetAve30U_Or30U.root\" \"exclusiveDijetsAnalysisTTree/ProcessedTree\" 6 3 4")

#----------------------------------------------------------->>>

print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''
