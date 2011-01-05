import FWCore.ParameterSet.Config as cms
import math

def pFlowId(name):
    types = ("X", "h", "e", "mu", "gamma", "h0", "h_HF", "egamma_HF")
    labels = {}
    labels["X"] = ("X","undefined")
    labels["h"] = ("h","chargedHadron","hadronCharged")
    labels["e"] = ("e","electron")
    labels["mu"] = ("mu","muon")
    labels["gamma"] = ("gamma","photon")
    labels["h0"] = ("h0","neutralHadron","hadronNeutral")
    labels["h_HF"] = ("h_HF","hadronHF")
    labels["egamma_HF"] = ("egamma_HF","emHF");

    id = -1
    for type in types:
        if name in labels[type]:
            id = types.index(type)
            break

    return id


class PFCandidateNoiseStringCut:
    def __init__(self, pset):
        self.pset = pset

        self.types = ("X", "h", "e", "mu", "gamma", "h0", "h_HF", "egamma_HF")
        self.ranges = {}
        self.ranges["Barrel"] = (0.0,1.4)
        self.ranges["Endcap"] = (1.4,2.6)
        self.ranges["Transition"] = (2.6,3.2)
        self.ranges["Forward"] = (3.2,999.0)       

    def cut(self):
        self.thresholdPSets = {}
        self.thresholdPSets["Barrel"] = self.pset.getParameter("Barrel")
        self.thresholdPSets["Endcap"] = self.pset.getParameter("Endcap")
        self.thresholdPSets["Transition"] = self.pset.getParameter("Transition")
        self.thresholdPSets["Forward"] = self.pset.getParameter("Forward")
 
        cutStr = ""
        for region in self.thresholdPSets:
            if cutStr: cutStr += " ) | ( "
            else: cutStr += "( "
            cutStr += "(eta >= %f & eta < %f)" % (self.ranges[region][0],self.ranges[region][1])

            regionPSet = self.thresholdPSets[region]
            typePSetNames = regionPSet.parameterNames_()
            typePSetIds = [pFlowId(item) for item in typePSetNames]
            cutStr += " & ( "
            idxType = 0
            for type in self.types:
                if idxType: cutStr += " ) | ( "
                else: cutStr += "( "
                typeId = pFlowId(type)   
                cutStr += "particleId == %d" % typeId
                if typeId in typePSetIds:
                    typePSet = regionPSet.getParameter( typePSetNames[typePSetIds.index(typeId)] )
                    variablePSetNames = typePSet.parameterNames_()
                    for variable in variablePSetNames:
                        value = typePSet.getParameter(variable).value()
                        cutStr += " & ( %s >= %f )" % (variable,value)
 
                idxType += 1
            cutStr += " )"
            cutStr += " )"

        cutStr += " )"      
        return cutStr   
