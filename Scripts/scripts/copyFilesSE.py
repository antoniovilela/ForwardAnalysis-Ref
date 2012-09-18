#!/usr/bin/python

import os,sys
from subprocess import Popen,PIPE,call
from ForwardAnalysis.Scripts.CopyWatch import CopyWatch

"""
#from threading import Thread,Lock,Event
from threading import Thread
class CopyWatch(Thread):
    def __init__(self,cmd):
        Thread.__init__(self)
        self.cmd = cmd
 
    def run(self):
        #print self.cmd
        call(self.cmd,shell=True)
"""

if __name__ == '__main__':
    import optparse
    usage = "usage: %prog [options] <remote dir> <local dir>"
    parser = optparse.OptionParser(usage=usage)
    parser.add_option("-s","--site", dest="site", metavar="SITE", help="site name")
    parser.add_option("-p","--port", dest="port", type="int", default=8443, metavar="PORT", help="SRM port (Default: 8443)")
    parser.add_option("-t","--type", dest="type", default="root", metavar="TYPE", help="select only files with substring TYPE (Default: 'root')")
    parser.add_option("--srm_str", dest="srmstr", default="/srm/v2/server?SFN=", metavar="SRM", help="SRM string (Default: '/srm/v2/server?SFN=')")
    parser.add_option("--to", dest="opt_to", action="store_true", default=False, help="will copy from local to remote dir")
    parser.add_option("--from", dest="opt_from", action="store_true", default=False, help="will copy from remote to local dir")
    parser.add_option("--no_exec", dest="enable", action="store_false", default=True, help="files will not be copied")

    (input, args) = parser.parse_args()

    if not input.site: parser.error('must set site name')
    if (input.opt_to and input.opt_from) or (not input.opt_to and not input.opt_from): parser.error('must set either "--to" or "--from"')
    if len(args) != 2: parser.error('exactly two arguments required')
    
    ###########
    srmlscmd = 'lcg-ls -b -D srmv2 --vo cms'
    srmcpcmd = 'lcg-cp -b -D srmv2 --vo cms'

    storage_path = ""
    localdir = ""
    if input.opt_from:
	storage_path = args[0] 
	localdir = args[1] 
    elif input.opt_to:
	storage_path = args[1] 
	localdir = args[0]
 
    #srm_string = '//srm/managerv2?SFN='
    #srm_string = '/srm/v2/server?SFN='
    srm_string = input.srmstr
    storage_name = input.site
    storage_port = input.port
    type = input.type
    endpoint = 'srm://' + storage_name + ':' + str(storage_port) + srm_string
    fullpath = endpoint + storage_path

    lscmd = ""
    if input.opt_from:
	lscmd = '%s "%s"' % (srmlscmd,fullpath)
    elif input.opt_to:
        lscmd = 'ls %s' % localdir
    print lscmd

    p1 = Popen(lscmd,shell=True,stdout=PIPE)
    files = [item.rstrip().split("/")[-1] for item in p1.stdout if item.find(type) != -1]

    if input.opt_to:
	mkdircmd = 'srmmkdir "%s"' % fullpath 
	print mkdircmd 
	#if input.enable: call(mkdircmd,shell=True)

    copyList = []
    for file in files:
        if input.opt_from:
	    outfile = os.path.abspath(localdir) + "/" + file
	    cmd = 'nice %s "%s/%s" "file:%s"' % (srmcpcmd,fullpath,file,outfile)
        elif input.opt_to:
	    inputfile = os.path.abspath(localdir) + "/" + file
	    cmd = 'nice %s "file:%s" "%s/%s"' % (srmcpcmd,inputfile,fullpath,file)
            
        print cmd
        #if input.enable: call(cmd,shell=True)
        if input.enable:
            copyList.append( CopyWatch(cmd) )
            copyList[-1].start()

    for item in copyList: item.join() 

    print "========================="
    print "----> Transfer completed."
    print "========================="
