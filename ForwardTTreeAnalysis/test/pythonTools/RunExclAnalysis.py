#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''

#  ./ExclDijetsComp "inputfile.root" "outputfile.root"  < pT(Jet1) Cut > < pT(Jet2) Cut >  < Number of Vertex Cut >  < Trigger Option > < Turn on(off) PU Reweight > < Turn on(off) Luminosity Reweight > < Turn on(off) Trigger Efficiency > < Turn on(off) event-per-event Weight > < Luminosity Weight Factor > < Trigger Efficiency Factor >
#
# TURN ON  = true
# TURN OFF = false
#
# I)   If you turn off PU reweight, the default weight will be 1;
# II)  If you turn off Luminosity reweight, the default weight will be 1;
# III) If you turn off Trigger Efficiency, the default weight will be 1;
# IV)  If you turn off event-per-event weight (some MC sample), the default weight will be 1;
#
# P.S.: Trigger Option. How to use?
#
#       from CMSSW CONFIG FILE:
#
#       config.hltPaths = ('HLT_ExclDiJet60_HFOR_v*','HLT_ExclDiJet60_HFAND_v*','HLT_Jet60_v*' )
#
# so the if you would like to use HLT_ExclDiJet60_HFAND_v*, the Trigger Option = 1.
#        if you would like to use HLT_Jet60_v*, the Trigger Option = 2.
#

############
# DATA
############

os.system("./ExclDijetsnoHFnoTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_noHFnoHLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

os.system("./ExclDijetsTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_HLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

os.system("./ExclDijetsHF50andTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_HF50andHLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

os.system("./ExclDijetsHF20andTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_HF20andHLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

os.system("./ExclDijetsHF10andTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_HF10andHLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

os.system("./ExclDijetsHF5andTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_HF5andHLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

os.system("./ExclDijetsHF50noTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_HF50noHLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

os.system("./ExclDijetsHF20noTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_HF20noHLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

os.system("./ExclDijetsHF10noTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_HF10noHLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

os.system("./ExclDijetsHF5noTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_HF5noHLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

os.system("./ExclDijetsEtaMaxEtaMin3andTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_EtaMaxEtaMin3andHLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

os.system("./ExclDijetsEtaMaxEtaMin3noTrigger_data \"/storage2/dmf/Common/OutputFiles/PatSamples2010_19_1_2012/PATMultijets2010B_19_1_12.root\" \"histo_patdata19_01_12_EtaMaxEtaMin3noHLT_excl_30Uand.root\" 60 60 1 0 0 0 0 0 1.0 1.0")

#----------------------------------------------------------->>>

print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''


