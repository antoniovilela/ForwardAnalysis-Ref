#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''

os.system("./RunTest \"/storage2/dmf/Common/OutputFiles/MCPythia6_third/QCD_Pt-15to3000-Z2_PUS3.root\" \"Test.root\" \"forwardQCDTTreeAnalysis/ProcessedTree\" 100.0 100.0 1 1 1 1 0 1 1 1 0 1.0 1.0")

os.system("./RunTest \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"TestData.root\" \"forwardQCDTTreeAnalysis/ProcessedTree\" 60 60 1 0 0 0 0 0 0 0 0 1.0 1.0")


#----------------------------------------------------------->>>

print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''


