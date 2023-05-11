import pigpio
import sys
import time
import traceback

class IR(object):
    def __init__(self, pin, io):
        self.pin = pin 
        self.io = io
        
        self.io.set_mode(self.pin, pigpio.INPUT)
        
        print("Sensors ready...")
        
    def read_IR(self):
        try:
            return self.io.read(self.pin)
        except BaseException as ex:
            # Report the error, then continue with the normal shutdown.
            print("Ending due to exception: %s" % repr(ex))
            traceback.print_exc()