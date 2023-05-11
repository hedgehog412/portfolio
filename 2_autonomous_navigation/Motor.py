import pigpio
import sys
import time
import traceback

class Motor(object):

    def __init__(self, Fpin, Rpin, side, io):

        self.io = io

        self.Fpin = Fpin
        self.Rpin = Rpin
        
        self.side = side

        # Set up the pins as output (commanding the motors).
        self.io.set_mode(self.Fpin, pigpio.OUTPUT)
        self.io.set_mode(self.Rpin, pigpio.OUTPUT)

        # Prepare the PWM.  The range gives the maximum value for 100%
        # duty cycle, using integer commands (1 up to max).
        self.io.set_PWM_range(self.Fpin, 255)
        self.io.set_PWM_range(self.Rpin, 255)

        # Set the PWM frequency to 1000Hz.
        self.io.set_PWM_frequency(self.Fpin, 1000)
        self.io.set_PWM_frequency(self.Rpin, 1000)

        # Clear all pins, just in case.
        self.turnOff()

        print("Motors ready...")

    def setPWM(self, v):
        value = v * self.side
        if value > 0:
            self.io.set_PWM_dutycycle(self.Fpin, abs(value))
            self.io.set_PWM_dutycycle(self.Rpin, 0)
        else:
            self.io.set_PWM_dutycycle(self.Rpin, abs(value))
            self.io.set_PWM_dutycycle(self.Fpin, 0)

    def turnOff(self):
        self.setPWM(0)