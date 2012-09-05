import FWCore.ParameterSet.Config as cms

class ExcludeHFEdgesStringCut:
    def __init__(self):
        #self.pset = pset
        self.ranges = []
        #self.ranges.append( (2.866,2.976) )
        # HF rings 29,30,40,41 - Acceptance 3.152 - 4.730
        self.ranges.append( (2.866,3.152) )
        self.ranges.append( (4.730,999.) )
    def cut(self):
 
        cutStr = ""
        for region in self.ranges:
            if cutStr: cutStr += " & "
            else: cutStr += "( "
            cutStr += "!(abs(eta) >= %f & abs(eta) < %f)" % (region[0],region[1])

        cutStr += " )"
        return cutStr   
