import time
import threading 
import Robot
from Intersection import *
from constants import * 
import ctypes 

actions = ['explore', 'pause', 'goal']

def run():
    #global robot_action
    global int_map
    
    while True:
        if Robot.stepping:
            temp_action = input("Enter step to continue or robot action to change action: ").strip()
        else:
            temp_action = input("Enter explore, pause, quit, goal, save or stepping to toggle stepping mode: ").strip()
        if temp_action in actions:
            Robot.robot_action = temp_action
            if Robot.robot_action == 'goal':
                new_goal = eval(input("Enter goal: "))
                while(type(new_goal) != tuple) or (new_goal not in int_map):
                    new_goal = eval(input("Invalid goal, enter new goal: "))
                    
                Robot.input_goal = new_goal
        elif temp_action == 'save':
            Robot.pickling(input("Enter file name: "))
        elif temp_action == 'quit':
            raise BaseException
        elif temp_action == 'stepping':
            Robot.stepping = not Robot.stepping
        elif temp_action == 'step':
            if Robot.stepping:
                Robot.step = True
                time.sleep(0.01)
                Robot.step = False
        elif temp_action == 'clear':
            Intersection.clear_blocks(int_map)
        else:
            Robot.robot_action = 'pause'
            


if __name__ == "__main__":
    #make robot thread, which makes the ultrasound thread
    print("Starting robot thread...")
    robot_thread = threading.Thread(name="RobotThread", target = Robot.robot_run)
    robot_thread.start()
    time.sleep(0.1)

    

    try:
        run()
    except:
        ctypes.pythonapi.PyThreadState_SetAsyncExc(ctypes.c_long(robot_thread.ident), ctypes.py_object(KeyboardInterrupt)) 
        
        print("Joining robot thread")
        robot_thread.join()
        print("robot thread joined")
