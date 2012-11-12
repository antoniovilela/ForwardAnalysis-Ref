import FWCore.ParameterSet.Config as cms

def replaceInPaths(process,old,new):
    for pathName in process.paths:
        getattr(process,pathName).replace(getattr(process,old),getattr(process,new))

    return process
