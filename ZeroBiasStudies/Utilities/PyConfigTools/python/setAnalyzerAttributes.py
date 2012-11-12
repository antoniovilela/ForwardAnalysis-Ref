import FWCore.ParameterSet.Config as cms

def setAnalyzerAttributes(process,string='',**pars):
    # Loop over analyzers
    for analyzerName in process.analyzers:
        if analyzerName.find(string) != -1:
            module = getattr(process,analyzerName)
            for attr in pars:
                setattr(module,attr,pars[attr]) 

    return process
