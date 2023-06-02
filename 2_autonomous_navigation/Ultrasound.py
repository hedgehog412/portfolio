import pigpio
import time

class Ultrasound(object):

    trig_high = 1
    trig_low = 0
    time_sleep = 0.00001
    trig_time_wait = 0.05
    speed_of_SOUND = 0.000339
    cbrise = None
    cbfall = None

    def __init__(self, io, pintrig, pinecho):
        self.io = io
        self.pintrig = pintrig
        self.pinecho = pinecho

        io.set_mode(pintrig, pigpio.OUTPUT)
        io.set_mode(pinecho, pigpio.INPUT)

        cbrise = io.callback(pinecho, pigpio.RISING_EDGE, self.rising)
        cbfall = io.callback(pinecho, pigpio.FALLING_EDGE, self.falling)

        self.last_trig_time = -1

        self.risetick = None

        self.distance = -1

    def trigger(self):
        curr_time = time.time()

        if(curr_time - self.last_trig_time > Ultrasound.trig_time_wait):
            self.io.write(self.pintrig, Ultrasound.trig_high)
            time.sleep(Ultrasound.time_sleep)
            self.io.write(self.pintrig, Ultrasound.trig_low)
            self.last_trig_time = curr_time

    def rising(self, pin, level, ticks):
        self.risetick = ticks

    def falling(self, pin, level, ticks): 
        deltatick = ticks - self.risetick 
        if (deltatick < 0):
            deltatick += 2 ** 32

        self.distance = deltatick * Ultrasound.speed_of_SOUND / 2.0
        #print(deltatick)

    def read(self):
        return self.distance
