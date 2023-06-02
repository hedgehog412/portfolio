import pigpio
import sys
import time
import traceback
from Motor import *

class Drive(object):
    def __init__(self, m1a, m1b, m2a, m2b, io):
        self.motorL = Motor(m1a, m1b, 1, io)
        self.motorR = Motor(m2a, m2b, -1, io)
        
    def drive(self, intensity, dir):
        left = 1
        right = 0

        # This number adjusts for the different speeds that the left and right wheels turn
        # when given the same pwm input. This is probably because we have motors with 
        # different gear ratios :(
        adjust = 10

        if dir == left:
            motor_slow = self.motorL
            motor_fast = self.motorR
        elif dir == right:
            motor_slow = self.motorR
            motor_fast = self.motorL
        else:
            raise Exception("Invalid direction")
        
        if intensity == "STRAIGHT":
            self.motorL.setPWM(210)
            self.motorR.setPWM(210)
                
        elif intensity == "VEER":
            motor_fast.setPWM(210)
            motor_slow.setPWM(180-(dir-1)*1.5*adjust)
                
        elif intensity == "STEER":
            motor_fast.setPWM(210)
            motor_slow.setPWM(125+dir*adjust)
                
        elif intensity == "TURN":
            motor_fast.setPWM(220)
            motor_slow.setPWM(100+dir*adjust)
                
        elif intensity == "HOOK":
            motor_fast.setPWM(230+dir*adjust*0.5)
            motor_slow.setPWM(0)
                
        elif intensity == "SPIN":
            #motor_fast.setPWM(205)
            #motor_slow.setPWM(-205+(dir-1)*adjust*1.5)

            motor_fast.setPWM(205)
            motor_slow.setPWM(-215 - 3 * (1 - dir) * adjust)

            # print(-205+(dir-1)*adjust*1.5)
        
        elif intensity == "BACK":
            self.motorL.setPWM(-220)
            self.motorR.setPWM(-210)




        elif intensity == "STOP":
            motor_fast.turnOff()
            motor_slow.turnOff()

        
                
        else:
            raise Exception("Not defined intensity")
        
    def set_PWM(self, pwm_l, pwm_r):
        self.motorL.setPWM(pwm_l)
        self.motorR.setPWM(pwm_r)
