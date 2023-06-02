import pigpio
import sys
import time
import traceback
from Drive import *
from LineSensor import *
from proximitysensor import *
from test import test
from Intersection import *
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import random
from constants import *
import pickle
import bisect
import math

curr_goal = None
robot_action = None 
input_goal = None
# recalc_dijkstra = True
stepping = False
step = False

def reset_costs():
    """
    Resets our intersection map to run Dijkstra's again
    """

    global int_map

    for key in int_map:
        int_map[key].cost = float("inf")
        int_map[key].dir = None        

def filter(prev, dt, u, T):
    """
    Implements our filter
    """
    val = prev + (dt / T) * (u - prev)
    return val
    
def detect(prev_filter2, dt, reading, T):    
    """
    Applies filter to all three sensor values for intersection detection
    """
    f1 = filter(prev_filter2[0], dt, reading[0], T)
    f2 = filter(prev_filter2[1], dt, reading[1], T)
    f3 = filter(prev_filter2[2], dt, reading[2], T)
        
    return (f1, f2, f3)

def threshold(reading, prev_reading, ut, lt, high_value, low_value):
    """ 
    Computes the sensor values using the defined thresholds
    """
    final_val = prev_reading
    
    if reading >= ut:
        final_val = high_value
    elif reading <= lt:
        final_val = low_value

    return final_val

def vic_dance(drive):
    """
    Completes a victory dance!!!
    """
    drive.drive("STRAIGHT", LEFT)
    time.sleep(0.1)
    drive.drive("BACK", RIGHT)
    time.sleep(0.1)
    drive.drive("STRAIGHT", LEFT)
    time.sleep(0.1)
    drive.drive("BACK", RIGHT)
    time.sleep(0.1)



def determine_angle(drive, sensor, p_sensor):
    """
    Determines the angle to turn at a given intersection, as described in Goals 6
    Returns -1 for right, 1 for left, or 0 for straight. 

    If the map is completed, will do the victory dance!
    """

    global curr_goal
    global curr_intersection
    global curr_position
    global int_map
    global curr_angle
    global first
    global sleep_time
    global trig_sleep_time

    direction = -2

    # Turn towards and unknown street
    for i in range(1, 3):
        if curr_intersection.neighbors[(curr_angle - i) % 8] == Intersection.UNKNOWN:
            direction = -1
            break
        elif curr_intersection.neighbors[(curr_angle + i) % 8] == Intersection.UNKNOWN:
            direction = 1
            break

    # if both left and right is explored, go straight
    time.sleep(trig_sleep_time)
    if direction == -2 and curr_intersection.neighbors[curr_angle] == Intersection.UNKNOWN and not check_is_blocked(p_sensor.read()[1]):
        direction = 0
        # if first:
        #     drive.drive("STRAIGHT", RIGHT)
        #     time.sleep(sleep_time)
        #     drive.drive("STOP", RIGHT)
        #     first = False

    
    raw_reading = sensor.read_line()
    # find the nearest unknown if everything is explored
    while(direction == 0 and raw_reading == (0, 0, 0)):
        int_map[curr_position].neighbors[curr_angle] = Intersection.DNE
        direction = -2

    # If we haven't found any unknowns... call mini dijkstra
    if curr_position == curr_goal: 
        curr_goal = None
    
    if direction == -2: 
        if curr_intersection.dir == None or curr_goal == None: 
            next_int = dijkstra(curr_position, True)
            curr_goal = next_int

            if next_int == None: 
                return 0

            dijkstra(next_int)

        dir_list = [(curr_angle + 1) % 8, (curr_angle + 2) % 8, (curr_angle + 3) % 8]
        
        if curr_intersection.dir == curr_angle:
            direction = 0
        elif curr_intersection.dir == (curr_angle + 4) % 8: # If want to do 180 turn, check turn w no interference
            if type(curr_intersection.neighbors[(curr_angle + 1) % 8]) != Intersection and type(curr_intersection.neighbors[(curr_angle + 2) % 8]) != Intersection:
                direction = 1
            elif type(curr_intersection.neighbors[(curr_angle - 1) % 8]) != Intersection and type(curr_intersection.neighbors[(curr_angle - 2) % 8]) != Intersection:
                direction = -1
            else:
                direction = -1
        elif curr_intersection.dir in dir_list:
            direction = 1
        else:
            direction = -1

    return direction

def calc_angle(curr_time, begining_time, first_time):
    """
    Calculates the angle turned
    """
    curr_time = time.time()
    turn_time = curr_time - begining_time
    angle_dt = curr_time - first_time

    angle_dt = max(0.08, angle_dt)

    angle = bot_angle
    angular_speed = angle/angle_dt

    raw_ret = (angular_speed * turn_time *57.2957795) 
    
    ret = int((raw_ret + 22.5) / 45) * 45

    if not (abs(ret) == 45):
        if abs(raw_ret) - abs(ret) > time_error:
            angle2 = abs(ret) + 45

            t1 = abs(turn_time - angle_times[abs(ret)])
            t2 = abs(turn_time - angle_times[abs(angle2)])

            if (t1 > t2):
                ret = angle2

        elif abs(raw_ret) - abs(ret) < -(time_error):
            angle2 = abs(ret) - 45

            t1 = abs(turn_time - angle_times[abs(ret)])
            t2 = abs(turn_time - angle_times[abs(angle2)])

            if (t1 > t2):
                ret = angle2
    if ret>180:
        ret = 180
    elif ret == 135:
        ret = 90
        angle2 = 180
        t1 = abs(turn_time - angle_times[abs(ret)])
        t2 = abs(turn_time - angle_times[abs(angle2)])

        if (t1 > t2):
            ret = angle2
    print("turntime: ", turn_time)
    return ret

def check_is_blocked(p_reading):
    """
    Checks if the current street the robot is facing is blocked. 
    Returns True if it is blocked, False otherwise
    """
    global curr_angle

    is_blocked = (curr_angle % 2 == 1 and p_reading < diag_distance) or (curr_angle % 2 == 0 and p_reading < straight_distance)

    if is_blocked: 
        print("Blocked street")
        int_map[curr_position].add_neighbor(curr_angle, Intersection.UNDRIVEN)
        int_map[curr_position].add_neighbor((curr_angle - 1) % 8, Intersection.DNE)
        int_map[curr_position].add_neighbor((curr_angle + 1) % 8, Intersection.DNE)
   
    return is_blocked

def turning_filtering(drive, sensor, p_sensor, direction, on_street = False):
    """
    Turns the robot until the next intersection and returns the angle turned
    """

    global prev_filter
    global prev_dir_filter
    global curr_position
    global curr_angle
    global time_error
    global curr_intersection  
    global reading
    global robot_action
    global first
    global trig_sleep_time

    if stepping:
        while not step and stepping:
            pass
    
    if (robot_action == 'pause'):
        drive.drive('STOP', RIGHT)

        while True:
            if robot_action == 'explore':
                break
            elif robot_action != 'pause' and robot_action != 'explore':
                return (False, 0, 0)
        
    if direction < 0: 
        drive.drive("SPIN", RIGHT)
        index = 2

    elif direction == 0:
        return (False, 0, 0)

    else: 
        drive.drive("SPIN", LEFT)
        index = 0

    first_time = -1.0
    turn_time = -1.0

    angle_dt = 0.0
    angle = 0.0
    angular_speed = 0.0

    detect_dir = 1
    begining_time = time.time()  
    prev_time = begining_time

    flag = 0
    curr_mid = 1

    while(True):
        curr_time = time.time()
        dt = curr_time - prev_time

        # current raw sensor reading
        raw_reading = sensor.read_line()
        pure_filter = detect(prev_filter, dt, raw_reading, turn_T)
        
        reading = tuple([threshold(curr, prev, upper_threshold_turn, lower_threshold_turn, 1, 0) for curr, prev in zip(pure_filter, reading)])
        
        if reading[1] == 0 and flag != 1:
            flag = 1
        
        if flag == 1 and reading[1] == 1 and first_time != -1:
            ret = calc_angle(time.time(), begining_time, first_time)

            prev_filter = (0, 1, 0)
            prev_dir_filter = 0
            
            drive.drive("STOP", 1)

            if not on_street:
                for i in range(1, 4):
                    curr_neighbor = int_map[curr_position].neighbors[(curr_angle + direction * i) % 8]
                    if curr_neighbor == "UNKNOWN":
                        break
                    elif type(curr_neighbor) == Intersection:
                        ret = i * 45
                        break

            update_map(direction * ret, on_street)

            # Check if street is blocked, return

            time.sleep(trig_sleep_time)
            print("angle: ", direction * ret)

            return (check_is_blocked(p_sensor.read()[1]), direction, direction * ret)
        
        if flag == 1 and reading[index] == 1 and first_time == -1:
            first_time = time.time()
            if direction < 0: 
                drive.drive("SPIN", RIGHT)
                index = 2
            else: 
                drive.drive("SPIN", LEFT)
                index = 0

        prev_filter = pure_filter
        prev_time = curr_time
    


def intersection_handle(drive, sensor, p_sensor):
    """
    Called once a robot detects an intersection. Returns the angle turned. 
    (right is negative, left is positive, staraight is 0)
    """

    global prev_filter
    global prev_dir_filter
    global curr_position
    global curr_angle
    global time_error
    global curr_intersection
    
    drive.drive("STOP", RIGHT)
    
    curr_intersection = int_map[curr_position]

    reading = sensor.read_line()
    
    prev_time = time.time()

    # Previous raw sensor reading
    reading = (0, 0, 0)

    # X_n-1
    prev_filter = (1,1,1)

    raw_reading = sensor.read_line()

    direction = determine_angle(drive, sensor, p_sensor)

    # turning filter
    return turning_filtering(drive, sensor, p_sensor, direction)
        
def line_following(drive, sensor, p_sensor):
    """
    Computes filter values to follow the line. 
    """
    global prev_dir_filter
    global prev_filter
    global reading
    global dir_filter
    global dir_reading
    global pure_filter
    global err_distance
    global curr_angle

    prev = time.time()
    dt = 0
    pure_filter = (0,1,0)
    prev_filter = (0,1,0)
    dir_filter = 0
    print(curr_angle)
    while True:
        curr = time.time()
        dt = curr - prev
        
        p_reading = p_sensor.read()
        if check_is_blocked(p_reading[1]):
            print("sudden obstacle")
            drive.drive("STOP", 1)
            return False
        
        raw_reading = sensor.read_line()
        if raw_reading == (1, 0, 0):
            dir_filter = filter(prev_dir_filter, dt, -1, standard_T)
        elif raw_reading == (1, 1, 0):
            dir_filter = filter(prev_dir_filter, dt, -0.5, standard_T)
        elif raw_reading == (0, 1, 1):
            dir_filter = filter(prev_dir_filter, dt, 0.5, standard_T)
        elif raw_reading == (0, 0, 1):
            dir_filter = filter(prev_dir_filter, dt, 1, standard_T)


        # 0 and 1 values based on X_n values
        pure_filter = detect(prev_filter, dt, raw_reading, peace_T)
        reading = tuple([threshold(curr, prev, upper_threshold, lower_threshold, 1, 0) for curr, prev in zip(pure_filter, reading)])
        dir_reading = threshold(dir_filter, 0, upper_threshold, lower_threshold, 1, -1)

        if (reading == (1, 1, 1)):
            drive.drive('STOP', 1)
            return True
            
        else:
            if (raw_reading == (0,1,0)) or (raw_reading == (1, 1, 1)):
                drive.drive("STRAIGHT", LEFT)
        
            elif (raw_reading == (1, 0, 0)):
                drive.drive("HOOK", LEFT)

            elif (raw_reading == (0, 0, 1)):
                drive.drive("HOOK", RIGHT)
            elif (raw_reading == (1, 1, 0)):
                drive.drive("HOOK", LEFT)
            elif (raw_reading == (0, 1, 1)):
                drive.drive("HOOK", RIGHT)
            elif(raw_reading == (0, 0, 0) and dir_reading > 0):
                while (raw_reading == (0, 0, 0)):
                    drive.drive("HOOK", RIGHT)
                    raw_reading = sensor.read_line()

            elif(raw_reading == (0, 0, 0) and dir_reading < 0):
                while (raw_reading == (0, 0, 0)):
                    drive.drive("HOOK", LEFT)
                    raw_reading = sensor.read_line()

            elif(raw_reading == (0, 0, 0)): 
                drive.drive("BACK", RIGHT)
                time.sleep(0.4)
            
        prev = curr
        prev_filter = pure_filter
        prev_dir_filter = dir_filter

def pos_from_angle(curr_position, curr_angle):
    """
    Return position from angle
    """
    x = curr_position[0]
    y = curr_position[1]
    new_position = curr_position
    if curr_angle == 0:
        new_position = (x + 1, y)
    elif curr_angle == 1:
        new_position = (x + 1, y + 1)
    elif curr_angle == 2:
        new_position = (x, y + 1)
    elif curr_angle == 3:
        new_position = (x - 1, y + 1)
    elif curr_angle == 4:
        new_position = (x - 1, y)
    elif curr_angle == 5:
        new_position = (x - 1, y - 1)
    elif curr_angle == 6:
        new_position = (x, y - 1)
    elif curr_angle == 7:
        new_position = (x + 1, y - 1)
    return new_position

    
def update_position():
    """
    Updates the global current_position variable given the current_angle variable. 
    """
    global curr_angle
    global curr_position
    
    curr_position = pos_from_angle(curr_position, curr_angle)

def angle_to_header(angle):
    """
    Converts an angle to a header
    """
    return int ((angle / 45) % 8)
    
def update_angle(angle):
    """
    Updates the current_angle field
    """
    global curr_angle
    if angle is None:
        return
    curr_angle = int (curr_angle + angle_to_header(angle)) % 8

def update_before(sensor):
    """
    Updates the map before the robot makes a turn. 
    """

    global curr_angle
    global curr_position

    prev_pos = curr_position
    
    prev_angle = curr_angle

    update_position()

    if (curr_position not in int_map):
        int_map[curr_position] = Intersection(curr_position[0], curr_position[1])

    if (sensor.read_line() == (0, 0, 0)):
        int_map[curr_position].add_neighbor(curr_angle, Intersection.DNE)

    int_map[curr_position].add_neighbor((prev_angle + 4) % 8, int_map[prev_pos])
    int_map[curr_position].add_neighbor((prev_angle + 5) % 8, Intersection.DNE)
    int_map[curr_position].add_neighbor((prev_angle + 3) % 8, Intersection.DNE)
    
    int_map[prev_pos].add_neighbor(prev_angle, int_map[curr_position])
    int_map[prev_pos].add_neighbor((prev_angle + 1) % 8, Intersection.DNE)
    int_map[prev_pos].add_neighbor((prev_angle - 1) % 8, Intersection.DNE)
    
    show_map()

def update_map(angle, on_street = False): 
    """
    Updates the map after a robot makes a turn
    """
    global curr_angle
    global curr_position

    prev_pos = curr_position
    prev_angle = curr_angle
    
    update_angle(angle)

    if not (angle is None) and not on_street:
        if angle > 0:
            for i in range(1, angle_to_header(abs(angle))):
                if int_map[curr_position].neighbors[(i + prev_angle) % 8] == Intersection.UNKNOWN:
                    int_map[curr_position].add_neighbor((i + prev_angle) % 8, Intersection.DNE)
        elif angle < 0:
            for i in range(1, angle_to_header(abs(angle))):
                if int_map[curr_position].neighbors[(prev_angle - i) % 8] == Intersection.UNKNOWN:
                    int_map[curr_position].add_neighbor((prev_angle - i) % 8, Intersection.DNE)   
    #show_map()     

def show_map():
    """
    Creates map based on intersection map
    """
    plt.cla()
    plt.clf()
    plt.close()

    fig, ax = plt.subplots(nrows=1, ncols=1)

    for coord in int_map:
        for i in range(8):
            neighbor = int_map[coord].neighbors[i]
            if type(neighbor) == Intersection:
                plt.plot((coord[0], neighbor.x), (coord[1], neighbor.y), "k-")
            elif neighbor == Intersection.DNE:
                new_pos = pos_from_angle(coord, i)
                plt.plot((coord[0], coord[0] + (new_pos[0] - coord[0]) / 10), (coord[1], coord[1] + (new_pos[1] - coord[1]) / 10), "r:")
            elif neighbor == Intersection.UNKNOWN:
                new_pos = pos_from_angle(coord, i)
                plt.plot((coord[0], coord[0] + (new_pos[0] - coord[0]) / 10), (coord[1], coord[1] + (new_pos[1] - coord[1]) / 10), "y:")
            elif neighbor == Intersection.UNDRIVEN:
                new_pos = pos_from_angle(coord, i)
                plt.plot((coord[0], new_pos[0]), (coord[1],new_pos[1]), "c:")
    
    global curr_position
    global curr_angle

    height = 0.1
    width = 0.2

    ax.add_patch(patches.Rectangle((curr_position[0]-width/2, curr_position[1]-height/2), width, height, angle = curr_angle*45, rotation_point = 'center'))            
    
    plt.savefig("maps/curr_map1.png")

def pickling(filename):
    global int_map

    print("Saving map to %s..." % filename)
    with open(filename, 'wb') as file:
        pickle.dump(int_map, file)

def loading_map(filename):
    global int_map

    print("Loading map from %s..." % filename)
    with open(filename, 'rb') as file:
        int_map = pickle.load(file)    
        
def euclid(pos1, pos2):
    return math.sqrt(((pos2[0]-pos1[0])**2) + ((pos2[1]-pos1[1])**2))
        
def dijkstra(goal, ret_unknown = False):

    global int_map
    global curr_position
    
    
    reset_costs()

    int_map[goal].cost = 0
    
    onDeck = [int_map[goal]]
    
    while len(onDeck) > 0:
        curr_node = onDeck.pop(0)

        for i in range(8):
            pot_cost = curr_node.cost
            
            neighbor = curr_node.neighbors[i]

            if ret_unknown and (neighbor == Intersection.UNKNOWN or (neighbor == Intersection.UNDRIVEN and not curr_node.neighbors_blocked[i])):
                return (curr_node.x, curr_node.y)
            
            if type(neighbor) == Intersection and not curr_node.neighbors_blocked[i]:
                pot_cost += euclid((curr_node.x, curr_node.y), (neighbor.x, neighbor.y))

                if pot_cost < neighbor.cost and not neighbor.neighbors_blocked[int ((i + 4) % 8)]:
                    if neighbor.cost < float('inf'):
                        onDeck.remove(neighbor)
                        
                    neighbor.cost = pot_cost
                    neighbor.dir = int ((i + 4) % 8)
                    bisect.insort(onDeck, neighbor)

    if ret_unknown: 
        return None
                
def intersection_dijkstra(drive, sensor, p_sensor):
    """
    Called once a robot detects an intersection. Returns the angle turned. 
    (right is negative, left is positive, staraight is 0)
    """

    global prev_filter
    global prev_dir_filter
    global curr_position
    global curr_angle
    global time_error
    global input_goal
    
    drive.drive("STOP", RIGHT)
    
    curr_intersection = int_map[curr_position]

    reading = sensor.read_line()
    
    prev_time = time.time()

    # Previous raw sensor reading
    reading = (0, 0, 0)

    # X_n-1
    prev_filter = (1,1,1)
    
    flag = 0
    curr_mid = 1

    direction = -2
    raw_reading = sensor.read_line()
    
    dir_list = [(curr_angle + 1) % 8, (curr_angle + 2) % 8, (curr_angle + 3) % 8]
    
    if curr_intersection.dir == curr_angle:
        direction = 0
    elif curr_intersection.dir == (curr_angle + 4) % 8: # If want to do 180 turn, check turn w no interference
        if type(curr_intersection.neighbors[(curr_angle + 1) % 8]) != Intersection and type(curr_intersection.neighbors[(curr_angle + 2) % 8]) != Intersection:
            direction = 1
        elif type(curr_intersection.neighbors[(curr_angle - 1) % 8]) != Intersection and type(curr_intersection.neighbors[(curr_angle - 2) % 8]) != Intersection:
            direction = -1
        else:
            direction = -1
    elif curr_intersection.dir in dir_list: # If want to turn left. turn left
        direction = 1
    else:
        direction = -1
        

    #variables for turning 
    return turning_filtering(drive, sensor, p_sensor, direction)

def turn_after_blocked():
    global curr_position
    global curr_angle
    global int_map
    
    direction = -1
    
    curr_intersection = int_map[curr_position]
    for i in range(1, 3):
        if type(curr_intersection.neighbors[(curr_angle - i) % 8]) == Intersection:
            direction = -1
            break
        elif type(curr_intersection.neighbors[(curr_angle + i) % 8]) == Intersection:
            direction = 1
            break
    return direction

def run(drive, sensor, p_sensor):

    """
    Main drive loop that gets sensor readings, updates map, and haldels intersections
    """
    
    prev_time = time.time()
    global curr_angle
    global curr_position
    global input_goal
    global robot_action
    # global recalc_dijkstra
    global int_map
    # Previous raw sensor reading
    
    first = True

    while True:

        pure_filter = (0,0,0)

        if robot_action != None and robot_action != 'pause' and not (robot_action == 'goal' and input_goal == None):
            # first = False
            if not line_following(drive, sensor, p_sensor):
                first = False
                turned = turning_filtering(drive, sensor, p_sensor, 1, on_street = True)
                if turned[2] != 180:
                    print("U turn not U turn")
                if not line_following(drive, sensor, p_sensor):
                    raise Exception("Blocked both sides of streetㅠㅠ")                    
            
            drive.drive("STRAIGHT", RIGHT)
            time.sleep(sleep_time)
            drive.drive("STOP", RIGHT)
            
            if first:
                update_before(sensor)
            else:
                first = True

        if robot_action == 'goal':                  
            # if recalc_dijkstra: 
            while robot_action == 'goal' and input_goal == None:
                drive.drive('STOP', 1)
            
            if robot_action == 'goal': 
                dijkstra(input_goal)
                    # recalc_dijkstra = False
                    
                turned = intersection_dijkstra(drive, sensor, p_sensor)

                is_blocked = turned[0]
                direction = turned[1]
                
                while is_blocked:
                    drive.drive('STOP', 1)
                    direction = turn_after_blocked()
                    turned = turning_filtering(drive, sensor, p_sensor, direction)
                    is_blocked = turned[0]
                    direction = turned[1]

                if curr_position == input_goal:
                    drive.drive('STOP', 1)
                    input_goal = None
                    robot_action = 'pause'
                    # first = False
                    print('Reached goal!')

        else: 
            input_goal = None 

            if robot_action == 'pause':
                while robot_action == 'pause':
                    drive.drive('STOP', 1)

            elif robot_action == 'explore' and not Intersection.map_complete(int_map): # and not first:
                turned = intersection_handle(drive, sensor, p_sensor)

                # while(is_blocked):
                #     drive.drive('STOP', 1)
                #     first = False
                #     is_blocked = intersection_handle(drive, sensor, p_sensor)

                is_blocked = turned[0]
                direction = turned[1]
                
                while is_blocked:
                    drive.drive('STOP', 1)
                    turned = turning_filtering(drive, sensor, p_sensor, direction)
                    is_blocked = turned[0]
                    direction = turned[1]

            elif Intersection.map_complete(int_map):
                drive.drive('STOP', 1)
                curr_intersection = int_map[curr_position]
                direction = turn_after_blocked()
                turning_filtering(drive, sensor, p_sensor, direction)
                robot_action = 'pause'
        
        show_map()
        
def robot_run():

    io = pigpio.pi()
    if not io.connected:
        print("Unable to connection to pigpio daemon!")
        sys.exit(0)
    print("GPIO ready...")
    
    drive = Drive(PIN_MOTOR1_LEGA, PIN_MOTOR1_LEGB, PIN_MOTOR2_LEGA, PIN_MOTOR2_LEGB, io)

    p_sensor = ProximitySensor(io, L_TRIG, L_ECHO, M_TRIG, M_ECHO, R_TRIG, R_ECHO)

    sensor = LineSensor(PIN_IR_LEFT, PIN_IR_MIDDLE, PIN_IR_RIGHT, io)

    global curr_position
    global int_map
    
    drive.drive("STOP", 0)
    
    try:
        # initilaize our first intersection in our map
        int_map[curr_position] = Intersection(curr_position[0], curr_position[1])

        while True:
            run(drive, sensor, p_sensor)

            
    except Exception as ex:
        # Report the error, then continue with the normal shutdown.
        print("Ending due to exception: %s" % repr(ex))
        traceback.print_exc()
        p_sensor.shutdown()
        
        # Shutdown cleanly (stopping the motors, then stopping the io).
        
    finally:
        drive.drive("STOP", 1)
        io.stop()


