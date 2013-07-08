import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
diffractiveZHLTFilter = copy.deepcopy(hltHighLevel)
diffractiveZHLTFilter.throw = False
diffractiveZHLTFilter.HLTPaths =['']
