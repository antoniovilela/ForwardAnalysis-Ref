#!/usr/bin/env python
import os



# Creating Configuration file for Monte Carlo
#-----------------------------------------------
def ConfigHandlerMCGrid(triggers,inputfile,outputfile,addition,useserver,dataset,ejob,nevents,uiworkingdir,usesavelocal,usedbsin,usedbsout,usese,
useadditional,sewhitelist,seblacklist,dbsin,dbsout,seelement,storagepath,sesave,publishname,port,mail):
    from pset_mc import mc_py
    mc_py = mc_py.replace('@@TRIGGERS@@',triggers,-1)
    mc_py = mc_py.replace('@@INPUT@@',inputfile,-1)
    mc_py = mc_py.replace('@@OUTPUT@@',outputfile,-1)
    mc_py = mc_py.replace('@@ADDITIONALFILE@@',addition,-1)
    mc_pyFile = open('pset.py','a+')
    mc_pyFile.write(mc_py)
    mc_pyFile.close()
    from crab_mc import cmc_cfg
    cmc_cfg = cmc_cfg.replace('@@ADDITIONALFILE@@',addition,-1)
    cmc_cfg = cmc_cfg.replace('@@USESERVER@@',useserver,-1)
    cmc_cfg = cmc_cfg.replace('@@DATASET@@',dataset,-1)
    cmc_cfg = cmc_cfg.replace('@@OUTPUT@@',outputfile,-1)
    cmc_cfg = cmc_cfg.replace('@@EJOB@@',ejob,-1)
    cmc_cfg = cmc_cfg.replace('@@NEVENTS@@',nevents,-1)
    cmc_cfg = cmc_cfg.replace('@@UIWORKINGDIR@@',uiworkingdir,-1)
    cmc_cfg = cmc_cfg.replace('@@USESAVELOCAL@@',usesavelocal,-1)
    cmc_cfg = cmc_cfg.replace('@@USEDBSIN@@',usedbsin,-1)
    cmc_cfg = cmc_cfg.replace('@@DBSSERVERIN@@',dbsin,-1)
    cmc_cfg = cmc_cfg.replace('@@USEDBSOUT@@',usedbsout,-1)
    cmc_cfg = cmc_cfg.replace('@@DBSSERVEROUT@@',dbsout,-1)
    cmc_cfg = cmc_cfg.replace('@@USESE_SAVE@@',usese,-1)
    cmc_cfg = cmc_cfg.replace('@@SEELEMENT@@',seelement,-1)
    cmc_cfg = cmc_cfg.replace('@@STORAGEPATH@@',storagepath,-1)
    cmc_cfg = cmc_cfg.replace('@@SE_SAVE@@',sesave,-1)
    cmc_cfg = cmc_cfg.replace('@@PUBLISHNAME@@',publishname,-1)
    cmc_cfg = cmc_cfg.replace('@@PORT@@',port,-1)
    cmc_cfg = cmc_cfg.replace('@@USEADDITIONALFILE@@',useadditional,-1)
    cmc_cfg = cmc_cfg.replace('@@SEBLACKLIST@@',seblacklist,-1)
    cmc_cfg = cmc_cfg.replace('@@SEWHITELIST@@',sewhitelist,-1)
    if (sewhitelist == ''):
        cmc_cfg = cmc_cfg.replace('@@USESEWHITELIST@@','#',-1)
    else:
        cmc_cfg = cmc_cfg.replace('@@USESEWHITELIST@@','',-1)
    if (seblacklist == ''):
        cmc_cfg = cmc_cfg.replace('@@USESEBLACKLIST@@','#',-1)
    else:
        cmc_cfg = cmc_cfg.replace('@@USESEBLACKLIST@@','',-1)
    cmc_cfg = cmc_cfg.replace('@@MAIL@@',mail,-1)
    cmc_cfgFile = open('crab.cfg','a+')
    cmc_cfgFile.write(cmc_cfg)
    cmc_cfgFile.close()
    if os.path.isfile('crab.cfg'):
       os.system('crab -create')
       os.system('cp crab.cfg '+uiworkingdir+'/.')
      # os.system('crab -submit -c '+ uiworkingdir)
       os.system('rm crab.cfg')
    if os.path.isfile('pset.py'):
       os.system('cp pset.py '+uiworkingdir+'/.')
       os.system('rm pset.py')
##---------------------------------------



triggers =  "'HLT_Jet30_v*','HLT_Jet60_v*', 'HLT_DiJetAve15U_v*', 'HLT_DiJetAve30U_v*', 'HLT_DiJetAve50U_v*', 'HLT_ExclDiJet60_HFAND_v*','HLT_ExclDiJet60_HFOR_v*'"
inputfile = ''
outputfile = 'qcd.root'
addition = '/storage2/eliza/lumibyXing_Cert_160404-176023_7TeV_PromptReco_Collisions11_JSON.root'
useserver = ''
dataset = '/QCD_Pt-0to5_TuneZ2_7TeV_pythia6/Summer11-PU_S3_START42_V11-v2/AODSIM'
ejob = '1'
nevents = '1'
uiworkingdir = 'crab_TestMC'
usesavelocal = ''
usedbsin =  '#'
usedbsout = '#'
usese = '#'
useadditional = '#' 
sewhitelist = ''
seblacklist = 'T3*,T1*'
dbsin = 'https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet'
dbsout = 'https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_02_writer/servlet/DBSServlet'
seelement = 'se-dcache.hepgrid.uerj.br'
storagepath = '/srm/managerv2?SFN=/pnfs/hepgrid.uerj.br/data/cms/store/user/dmf'
sesave = '/MyRemoteDirSE'
publishname = 'mctype_physics_globaltag_cmssw-version'
port = '8443'
mail = 'dmf@cern.ch'
ConfigHandlerMCGrid(triggers,inputfile,outputfile,addition,useserver,dataset,ejob,nevents,uiworkingdir,usesavelocal,usedbsin,usedbsout,usese,
useadditional,sewhitelist,seblacklist,dbsin,dbsout,seelement,storagepath,sesave,publishname,port,mail)

