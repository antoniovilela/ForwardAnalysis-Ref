import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
CommonHLTFilter = copy.deepcopy(hltHighLevel)
CommonHLTFilter.HLTPaths =['']
