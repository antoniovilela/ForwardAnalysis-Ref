#!/usr/bin/env python
import os

print ''
print '@@@@@@@@@@@@'
print 'Run Analysis'
print '@@@@@@@@@@@@'
print ''

os.system("./ExclusiveDijet \"/storage1/eliza/PATTuples_Mar2013/mc_HERWIG/mc_total/mc_herwig_lowpu_0.root\" \"histo_castor_collisions.root\" \"exclusiveDijetsAnalysisTTree\" \"no_trigger\" \"no_multiple_pileup\" \"plus\" \"pileup_correction\" \"pu_MultiJets_completejson_excldijets_march2013_68mb_bin24.root\" \"puHistoPythia6.root\" \"effMinBias2010RunB_hf7pf.root\" \"effTriggerMultijetsRunB_RefDijetAve50U_And30U_pT60_60.root\" \"no_cut_correction\" \"no_trigger_correction\" \"mc_lumi_weight\" 50.345 \"mc_event_weight\" 0 1 60 60")

#os.system("./ExclusiveDijet \"/storage1/dmf/Samples/PattuplesHF7/March_2013/ZeroBias_HF7_SL/zerobias_pf_hf7_march2013.root\" \"histo_castor_collisions.root\" \"exclusiveDijetsAnalysisZeroBiasTTree\" \"trigger\" \"no_multiple_pileup\" \"plus\" \"no_pileup_correction\" \"pu_MultiJets_completejson_excldijets_march2013_68mb_bin24.root\" \"puHistoPythia6.root\" \"effMinBias2010RunB_hf7pf.root\" \"effTriggerMultijetsRunB_RefDijetAve50U_And30U_pT60_60.root\" \"cut_correction\" \"trigger_correction\" \"no_mc_lumi_weight\" 50.345 \"no_mc_event_weight\" 0 1 60 60")


print ''
print '@@@@@@@@@@@@@@@@@@'
print 'Finishing Analysis'
print '@@@@@@@@@@@@@@@@@@'
print ''
