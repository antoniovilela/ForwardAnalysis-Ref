#!/usr/bin/env python
import os



# Creating Configuration file for Monte Carlo
#-----------------------------------------------
def ConfigHandlerMC(inputfile,outputfile,triggers,addition,uiworkingdir):
    from pset_mc import mc_py
    mc_py = mc_py.replace('@@TRIGGERS@@',triggers,-1)
    mc_py = mc_py.replace('@@INPUT@@',inputfile,-1)
    mc_py = mc_py.replace('@@OUTPUT@@',outputfile,-1)
    mc_py = mc_py.replace('@@ADDITIONALFILE@@',addition,-1)
    mc_pyFile = open('pset.py','a+')
    mc_pyFile.write(mc_py)
    mc_pyFile.close()
    if os.path.isfile('pset.py'):
       os.system('mkdir -p '+uiworkingdir+'/Backup')
       os.system('cp pset.py '+uiworkingdir+'/Backup/.')
       os.system('cmsRun '+uiworkingdir+'/Backup/pset.py')
       os.system('rm pset.py')
##---------------------------------------


inputfile = ''
outputfile = 'teste.root'
triggers = "'HLT_Jet30*','HLT_Jet60*','HLT_Jet80*','HLT_Jet110*','HLT_Jet150*','HLT_Jet190*','HLT_Jet240_v*','HLT_Jet370_v*'"
addition = '/storage2/eliza/lumibyXing_Cert_160404-176023_7TeV_PromptReco_Collisions11_JSON.root'
uiworkingdir = 'mcTest'
ConfigHandlerMC(inputfile,outputfile,triggers,addition,uiworkingdir)
