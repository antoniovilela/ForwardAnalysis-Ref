import FWCore.ParameterSet.Config as cms

def prependPaths(process,seqname):
    for path in process.paths: 
        getattr(process,path)._seq = getattr(process,seqname)*getattr(process,path)._seq
