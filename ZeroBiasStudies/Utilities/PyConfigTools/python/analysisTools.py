import FWCore.ParameterSet.Config as cms

def addAnalyzer(process,refAnalyzer,refSequence=None,filter="",**pars):
    analyzer = refAnalyzer.label()
    forbiddenStrings = ('.',)
    if filter:
        if filter[0] == "!":
            analyzer += "_Not" + filter[1:]
        else:
            analyzer += "_" + filter
    if refSequence:
        analyzer += "_" + refSequence.label()
    for key in pars:
        valueLabel = str(pars[key])
        for replaceStr in forbiddenStrings: valueLabel = valueLabel.replace(replaceStr,'_')
        analyzer += "_" + key + "_" + valueLabel

    if not hasattr(process,analyzer):
        setattr(process,analyzer,refAnalyzer.clone(**pars))

    seq = "analysis_" + analyzer
    sequence = cms.Sequence(getattr(process,analyzer))
    if filter:
        if filter[0] == "!":
            sequence.replace(getattr(process,analyzer),~getattr(process,filter[1:])*getattr(process,analyzer)) 
        else:
            sequence.replace(getattr(process,analyzer),getattr(process,filter)*getattr(process,analyzer))
  
    if refSequence: sequence.replace(getattr(process,analyzer),refSequence*getattr(process,analyzer))

    setattr(process,seq,sequence)

    return getattr(process,seq)

def addPath(process,sequence):
    path = sequence.label() + "_step"
    setattr(process,path,cms.Path(sequence))

def makeAnalysis(process,refAnalyzer='edmDumpAnalysis',refSequence='',attributes=[],filters=[]):
    analyzer = getattr(process,refAnalyzer)
    sequence = None
    if refSequence: sequence = getattr(process,refSequence)
    if not attributes and not filters: addPath(process,addAnalyzer(process,analyzer,sequence))

    for replace in attributes:
        addPath(process,addAnalyzer(process,analyzer,sequence,**replace))

    for item in filters:
        addPath(process,addAnalyzer(process,analyzer,sequence,item))
