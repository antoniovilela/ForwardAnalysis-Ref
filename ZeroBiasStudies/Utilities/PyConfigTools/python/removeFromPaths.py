import FWCore.ParameterSet.Config as cms

def removeFromPaths(process,module):
    for pathName in process.paths:
        getattr(process,pathName).remove(getattr(process,module))

    return process
