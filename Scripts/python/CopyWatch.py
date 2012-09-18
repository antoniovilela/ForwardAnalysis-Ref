#from threading import Thread,Lock,Event
from threading import Thread
from subprocess import call

class CopyWatch(Thread):
    def __init__(self,cmd):
        Thread.__init__(self)
        self.cmd = cmd
 
    def run(self):
        #print self.cmd
        call(self.cmd,shell=True)
