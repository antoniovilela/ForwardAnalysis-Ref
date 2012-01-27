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
# MonteCarlo
############

# event-per-event Weight = TRUE
# PU Reweight = TRUE
#

os.system("./ExclDijetsEtaMaxEtaMin3_MC \"/storage2/dmf/Common/OutputFiles/MCPythia6_third/QCD_Pt-15to3000-Z2_PUS3.root\" \"histo_pythia15to3000_PFEtaMaxEtaMin3andJet30.root\" 60 60 1 0 1 1 0 1 50550.5410428217 1.0")

os.system("./ExclDijetswithoutHFandJet_MC \"/storage2/dmf/Common/OutputFiles/MCPythia6_third/QCD_Pt-15to3000-Z2_PUS3.root\" \"histo_pythia15to3000_noHFandJet30.root\" 60 60 1 0 1 1 0 1 50550.5410428217 1.0")

os.system("./ExclDijetsHF5_MC \"/storage2/dmf/Common/OutputFiles/MCPythia6_third/QCD_Pt-15to3000-Z2_PUS3.root\" \"histo_pythia15to3000_HF5andJet30.root\" 60 60 1 0 1 1 0 1 50550.5410428217 1.0")

os.system("./ExclDijetsHF10_MC \"/storage2/dmf/Common/OutputFiles/MCPythia6_third/QCD_Pt-15to3000-Z2_PUS3.root\" \"histo_pythia15to3000_HF10andJet30.root\" 60 60 1 0 1 1 0 1 50550.5410428217 1.0")

os.system("./ExclDijetsHF20_MC \"/storage2/dmf/Common/OutputFiles/MCPythia6_third/QCD_Pt-15to3000-Z2_PUS3.root\" \"histo_pythia15to3000_HF20andJet30.root\" 60 60 1 0 1 1 0 1 50550.5410428217 1.0")

os.system("./ExclDijetsHF50_MC \"/storage2/dmf/Common/OutputFiles/MCPythia6_third/QCD_Pt-15to3000-Z2_PUS3.root\" \"histo_pythia15to3000_HF50andJet30.root\" 60 60 1 0 1 1 0 1 50550.5410428217 1.0")


#----------------------------------------------------------->>>

print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''


