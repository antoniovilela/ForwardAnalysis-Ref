from ForwardAnalysis.Skimming.prependPaths import prependPaths

from patTuple_PF2PAT_data_cfg import process

# Add HLT filter
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.hltFilter = copy.deepcopy(hltHighLevel)
process.hltFilter.HLTPaths = ['HLT_ExclDiJet60_HFAND_v*', 
                              'HLT_ExclDiJet60_HFOR_v*'
                              ]

# Add filter in paths
prependPaths(process,'hltFilter') 
