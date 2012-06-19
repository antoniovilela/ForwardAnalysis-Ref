import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
exclusiveDijetsHLTFilter = copy.deepcopy(hltHighLevel)
exclusiveDijetsHLTFilter.HLTPaths =['']
