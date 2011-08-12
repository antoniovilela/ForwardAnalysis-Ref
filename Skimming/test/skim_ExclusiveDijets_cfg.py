from ForwardAnalysis.Skimming.prependPaths import prependPaths

from patTuple_PF2PAT_data_cfg import process

# Add HLT filter
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.hltFilter = copy.deepcopy(hltHighLevel)
process.hltFilter.HLTPaths = ['HLT_*']

# Add filter in paths
prependPaths(process,'hltFilter') 
