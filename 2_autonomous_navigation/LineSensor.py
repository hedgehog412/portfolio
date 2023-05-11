import pigpio
import sys
import time
import traceback
from IR import *

class LineSensor(object):
    def __init__(self, lpin, mpin, rpin, io):
        self.l_ir = IR(lpin, io)
        self.m_ir = IR(mpin, io)
        self.r_ir = IR(rpin, io)
        
        self.io = io
        
    def read_line(self):
        return (self.l_ir.read_IR(), self.m_ir.read_IR(), self.r_ir.read_IR())
        
        

        