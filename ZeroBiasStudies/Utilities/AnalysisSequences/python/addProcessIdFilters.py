import FWCore.ParameterSet.Config as cms

def addProcessIdFilters(process,generator):
    allowedGenerators = ('Pythia6','Pythia8','Phojet')
    if generator not in allowedGenerators:
        errmsg  = 'Generator "%s" not valid.\n' % generator
        errmsg += '   Valid options are:\n'
        for item in allowedGenerators: errmsg += '        ' + item + '\n'
        raise RuntimeError,errmsg

    from Utilities.AnalysisTools.processIdFilter_cfi import processIdFilter
    processIdFilters = None 
    if generator == 'Pythia6': 
        process.processIdPythia6_SD = processIdFilter.clone(ProcessIds = [92,93])
        process.processIdPythia6_SDPlus = processIdFilter.clone(ProcessIds = [93])
        process.processIdPythia6_SDMinus = processIdFilter.clone(ProcessIds = [92])
        process.processIdPythia6_DD = processIdFilter.clone(ProcessIds = [94])
        process.processIdPythia6_Diff = processIdFilter.clone(ProcessIds = [92,93,94])
        process.processIdPythia6_ND = cms.Sequence(~process.processIdPythia6_Diff)
        
        processIdFilters = ['processIdPythia6_SD','processIdPythia6_SDPlus','processIdPythia6_SDMinus',
                            'processIdPythia6_DD',
                            'processIdPythia6_Diff','processIdPythia6_ND']
    elif generator == 'Pythia8':
        process.processIdPythia8_SD = processIdFilter.clone(ProcessIds = [103,104])
        process.processIdPythia8_SDPlus = processIdFilter.clone(ProcessIds = [104])
        process.processIdPythia8_SDMinus = processIdFilter.clone(ProcessIds = [103])
        process.processIdPythia8_DD = processIdFilter.clone(ProcessIds = [105])
        process.processIdPythia8_Diff = processIdFilter.clone(ProcessIds = [103,104,105])
        process.processIdPythia8_ND = cms.Sequence(~process.processIdPythia8_Diff)

        processIdFilters = ['processIdPythia8_SD','processIdPythia8_SDPlus','processIdPythia8_SDMinus',
                            'processIdPythia8_DD',
                            'processIdPythia8_Diff','processIdPythia8_ND']
    elif generator == 'Phojet':
        process.processIdPhojet_SD = processIdFilter.clone(GeneratorTag = "source", ProcessIds = [5,6])
        process.processIdPhojet_SDPlus = processIdFilter.clone(GeneratorTag = "source", ProcessIds = [5])
        process.processIdPhojet_SDMinus = processIdFilter.clone(GeneratorTag = "source", ProcessIds = [6])
        process.processIdPhojet_DD = processIdFilter.clone(GeneratorTag = "source", ProcessIds = [7])
        process.processIdPhojet_Diff = processIdFilter.clone(GeneratorTag = "source", ProcessIds = [4,5,6,7])
        process.processIdPhojet_ND = cms.Sequence(~process.processIdPhojet_Diff)

        processIdFilters = ['processIdPhojet_SD','processIdPhojet_SDPlus','processIdPhojet_SDMinus',
                            'processIdPhojet_DD',
                            'processIdPhojet_Diff','processIdPhojet_ND']

    return processIdFilters
