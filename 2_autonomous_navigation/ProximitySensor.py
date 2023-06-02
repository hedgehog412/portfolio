from ultrasound import Ultrasound
import pigpio
import time
import threading 

L_TRIG = 13
L_ECHO = 16
M_TRIG = 19
M_ECHO = 20
R_TRIG = 26
R_ECHO = 21

class ProximitySensor(object):
    def __init__(self,io,  l_trig, l_echo, m_trig, m_echo, r_trig, r_echo):
        
        self.l_us = Ultrasound(io, l_trig, l_echo)
        self.m_us = Ultrasound(io, m_trig, m_echo)
        self.r_us = Ultrasound(io, r_trig, r_echo)
        
        print("Starting triggering thread...")
        self.triggering = True 
        self.thread = threading.Thread(name="TriggeringThread", target = self.run)
        self.thread.start()
        time.sleep(0.1)

    def trigger(self):
        self.l_us.trigger()
        self.m_us.trigger()
        self.r_us.trigger()

    def read(self):
        return (self.l_us.read(), self.m_us.read(), self.r_us.read())

    def run(self):
        while self.triggering:
            self.trigger()
            time.sleep(0.05)
            
    def shutdown(self):
        self.triggering = False
        print("Waiting for triggering thread to finish...")
        self.thread.join()
        print("Triggering thread returned.")

if __name__ == "__main__":
    io = pigpio.pi()
    sensor = ProximitySensor(io, L_TRIG, L_ECHO, M_TRIG, M_ECHO, R_TRIG, R_ECHO)
    while True:
        sensor.trigger()
        time.sleep(0.050)
        distances = sensor.read()
        print("Distances = (%6.3fm, %6.3fm, %6.3fm)" % distances)
