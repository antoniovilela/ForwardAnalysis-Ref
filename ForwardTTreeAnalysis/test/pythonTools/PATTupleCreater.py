#!/usr/bin/env python
import os
import random
import time



#####################
# Defining Functions
#####################



# Running Data
#-----------------
def Data(runtype):
    print ''
    print '#############'
    print 'Running Data'
    print '#############'
    if runtype=="grid":
        print ''
        from data_samples_grid import ConfigHandlerDataGrid
        print ''
    elif runtype=="local":
        print ''
        from data_samples import ConfigHandlerData
        print ''
    else:
        print ''
        print 'Please, insert type grid or local.'
        print ''
##---------------------------------------



# Running MC
#--------------
def MC(runtype):
    print ''
    print '##########'
    print 'Running MC'
    print '##########'
    if runtype=="grid":
        print ''
        from mc_samples_grid import ConfigHandlerMCGrid
        print ''
    elif runtype=="local":
        print ''
        from mc_samples import ConfigHandlerMC
        print ''
    else:
        print ''
        print 'Please, insert type grid or local.'
        print ''
##---------------------------------------



#######################
# Program Configuration
#######################

runtype='local'
Data(runtype)

runtype='local'
MC(runtype)

print ''
print '@@@@@@@@@'
print 'Finishing'
print '@@@@@@@@@'
print ''


