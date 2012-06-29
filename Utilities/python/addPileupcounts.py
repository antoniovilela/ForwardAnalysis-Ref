import FWCore.ParameterSet.Config as cms

from Utilities.AnalysisTools.pileUpInfoFilter_cfi import pileUpInfoFilter
from Utilities.AnalysisTools.countsAnalyzer_cfi import countsAnalyzer

def addPileupcounts(process):
   for i in range(11):
      filterPU = 'filter%dPU'%i
      countsPU = 'counts%dPU'%i
      setattr(process,filterPU, pileUpInfoFilter.clone(numberOfPileUpEvents = i))
      setattr(process,countsPU, countsAnalyzer.clone())
      setattr(process,countsPU + '_step',cms.Path(getattr(process,filterPU) + getattr(process,countsPU)))

   return process
