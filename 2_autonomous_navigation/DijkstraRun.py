import pigpio
import sys
import time
import traceback
from Drive import *
from LineSensor import *
from test import test
from Intersection import *
import matplotlib.pyplot as plt
import random
from constants import *
import pickle
import bisect
import math

def reset_costs():
    """
    Resets our intersection map to run Dijkstra's again
    """


    global int_map

    for key in int_map:
        int_map[key].cost = float("inf")
        int_map[key].dir = None

def get_nearest_unknown():
    """
    Runs abbreviated Dijkstra's to find the nearest intersection with an 
    UNKNOWN value. Returns the location of this intersection, or None if the 
    map is complete. 
    """

    reset_costs()

    global int_map
    global curr_position

    int_map[curr_position].cost = 0
    
    onDeck = [int_map[curr_position]]
    
    while len(onDeck) > 0:
        curr_node = onDeck.pop(0)
        pot_cost = curr_node.cost
        for i in range(8):
            neighbor = curr_node.neighbors[i]

            if neighbor == Intersection.UNKNOWN:
                return (curr_node.x, curr_node.y)
            
            if type(neighbor) == Intersection:
                pot_cost += euclid((curr_node.x, curr_node.y), (neighbor.x, neighbor.y))

                print(neighbor.x)
                print(neighbor.dir)

                if pot_cost < neighbor.cost:
                    if neighbor.cost < float('inf'):
                        onDeck.remove(neighbor)
                        
                    neighbor.cost = pot_cost
                    neighbor.dir = int ((i + 4) % 8)
                    bisect.insort(onDeck, neighbor)

    return None            

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

def threshold(reading, prev_reading, ut, lt):
    """ 
    Computes the sensor values using the defined thresholds
    """
    final_vals = [0, 0, 0]
    
    for i in range(len(reading)): 
        if reading[i] >= ut:
            final_vals[i] = 1
        elif reading[i] <= lt:
            final_vals[i] = 0
        else: 
            final_vals[i] = prev_reading[i]

    return tuple(final_vals)

def dir_threshold(reading, prev_reading, ut, lt):
    """
    Similar to threshold, but works for the direction filter
    """
    final_val = 0
    
    if reading >= ut:
        final_val = 1
    elif reading <= lt:
        final_val = -1
    else:
        final_val = 0
    return final_val

def map_complete():
    """
    Returns true if the map is complete, false otherwise
    """
    for v in int_map.values():
        for neighbor in v.neighbors:
            if neighbor == Intersection.UNKNOWN:       
                return False
    return True

def vic_dance():
    """
    Completes a victory dance!!!
    """
    while (True):
        drive.drive("SPIN", LEFT)
        time.sleep(0.1)
        drive.drive("SPIN", RIGHT)
        time.sleep(0.1)

def intersection_handle(drive, sensor):
    """
    Called once a robot detects an intersection. Returns the angle turned. 
    (right is negative, left is positive, staraight is 0)
    """

    global prev_filter
    global prev_dir_filter
    global curr_position
    global curr_angle
    global time_error
    

    drive.drive("STRAIGHT", RIGHT)
    time.sleep(sleep_time)
    drive.drive("STOP", RIGHT)
    
    update_before()
    
    curr_intersection = int_map[curr_position]

    reading = sensor.read_line()
    
    prev_time = time.time()

    # Previous raw sensor reading
    reading = (0, 0, 0)

    # X_n-1
    prev_filter = (1,1,1)
    
    flag = 0
    curr_mid = 1
    index = -1

    direction = -2
    raw_reading = sensor.read_line()
    
    # print("curr_angle: ", curr_angle)
    
    # autonomous code
    # checks all intersection to the right, if unknown turns right
    for i in range(1, 4):
        if curr_intersection.neighbors[(curr_angle - i) % 8] == Intersection.UNKNOWN:
            # print("first", (curr_angle - i) % 8)
            direction = -1
            break
        elif type(curr_intersection.neighbors[(curr_angle - i) % 8]) == Intersection:
            break

    # if intersection to the right is already explored, check intersections to the left        
    if direction == -2:
        for i in range(1, 4):
            if curr_intersection.neighbors[(curr_angle + i) % 8] == Intersection.UNKNOWN:
                # print("second", (curr_angle + i) % 8)
                direction = 1
                break
            elif type(curr_intersection.neighbors[(curr_angle + i) % 8]) == Intersection:
                break

    # if both left and right is explored, go straight
    if direction == -2 and curr_intersection.neighbors[curr_angle] == Intersection.UNKNOWN:
        direction = 0
    
    
    # random turn left and right if everything is explored
    while(direction == 0 and raw_reading == (0, 0, 0)):
        int_map[curr_position].neighbors[curr_angle] = Intersection.DNE
        direction = -2

    # If we haven't found any unknowns... call mini dijkstra
    if direction == -2: 

        if curr_intersection.dir == None: 
            next_int = get_nearest_unknown()
            print(next_int)

            if next_int == None: 
                assert(map_complete())
                vic_dance()

            dijkstra(next_int)

        dir_list = [(curr_angle + 1) % 8, (curr_angle + 2) % 8, (curr_angle + 3) % 8, (curr_angle + 4) % 8]
        
        if curr_intersection.dir == curr_angle:
            direction = 0
        elif curr_intersection.dir in dir_list:
            direction = 1
        else:
            direction = -1

    # turning
    if direction < 0: 
        drive.drive("SPIN", RIGHT)
        index = 2

    elif direction == 0:
        return 0

    else: 
        drive.drive("SPIN", LEFT)
        index = 0

    #variables for turning 
    first_time = -1.0
    turn_time = -1.0

    angle_dt = 0.0
    angle = 0.0
    angular_speed = 0.0

    detect_dir = 1
    begining_time = time.time()

    # turning filter
    while(True):
        curr_time = time.time()
        dt = curr_time - prev_time

        # current raw sensor reading
        raw_reading = sensor.read_line()
        pure_filter = detect(prev_filter, dt, raw_reading, turn_T)
        
        reading = threshold(pure_filter, reading, upper_threshold_turn, lower_threshold_turn)
        
        if reading[1] == 0 and flag != 1:
            flag = 1
        
        if flag == 1 and reading[1] == 1 and first_time != -1:
            curr_time = time.time()
            # drive.drive("STOP", LEFT)
            # input("enter")
            turn_time = curr_time - begining_time
            angle_dt = curr_time - first_time

            angle_dt = max(0.08, angle_dt)

            angle = bot_angle
            angular_speed = angle/angle_dt

            raw_ret = (angular_speed * turn_time *57.2957795) 
            
            ret = int((raw_ret + 22.5) / 45) * 45
            
            turned = angle_to_header(ret)
                

            # if angles are close to borderline, use timing to tie break
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

            prev_filter = (0, 1, 0)
            prev_dir_filter = 0

            if ret>180:
                ret = 180
            elif ret == 135:
                ret = 90
                angle2 = 180
                t1 = abs(turn_time - angle_times[abs(ret)])
                # print(f"t1: {t1}")
                t2 = abs(turn_time - angle_times[abs(angle2)])
                # print(f"t2: {t2}")

                if (t1 > t2):
                    ret = angle2

            # print(f"final angle: {direction*ret}")
            
            tm = turn_time
            t1 = first_time - begining_time
            
            print("raw: ", raw_ret, " tm: ", tm, " t1: ", t1, " tm - t1: ", tm - t1, " tm/(tm - t1): ", tm / (tm - t1), " ret: ", direction * ret)
            if ret == 135:
                raise Exception("135 angle detected")
            return direction*ret

        
        if flag == 1 and reading[index] == 1 and first_time == -1:
            first_time = time.time()
            # drive.drive("STOP", LEFT)
            # input("Enter")
            if direction < 0: 
                drive.drive("SPIN", RIGHT)
                index = 2
            else: 
                drive.drive("SPIN", LEFT)
                index = 0

        prev_filter = pure_filter
        prev_time = curr_time
        
def line_following(drive, sensor, dt):
    """
    Computes filter values to follow the line. 
    """
    global prev_dir_filter
    global prev_filter
    global reading
    global dir_filter
    global dir_reading
    global pure_filter
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
    reading = threshold(pure_filter, reading, upper_threshold, lower_threshold)
    dir_reading = dir_threshold(dir_filter, dir_reading, upper_threshold, lower_threshold)
    # print(pure_filter)
    # print(pure_filter, raw_reading)

    if (reading == (1, 1, 1)):
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
        # elif(raw_reading == (0, 0, 0) and (reading == (0, 0, 1) or reading == (0, 1, 1))):
        elif(raw_reading == (0, 0, 0) and dir_reading > 0):
            # print(dir_reading)
            while (raw_reading == (0, 0, 0)):
                drive.drive("HOOK", RIGHT)
                raw_reading = sensor.read_line()

        # elif(raw_reading == (0, 0, 0) and (reading == (1, 0, 0) or reading == (1, 1, 0))):
        elif(raw_reading == (0, 0, 0) and dir_reading < 0):
            # print(dir_reading)
            while (raw_reading == (0, 0, 0)):
                drive.drive("HOOK", LEFT)
                raw_reading = sensor.read_line()

        elif(raw_reading == (0, 0, 0)): 
            drive.drive("STOP", RIGHT)
        

    prev_filter = pure_filter
    prev_dir_filter = dir_filter
    return False

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
    # x = curr_position[0]
    # y = curr_position[1]
    # if curr_angle == 0:
    #     curr_position = (x + 1, y)
    # elif curr_angle == 1:
    #     curr_position = (x + 1, y + 1)
    # elif curr_angle == 2:
    #     curr_position = (x, y + 1)
    # elif curr_angle == 3:
    #     curr_position = (x - 1, y + 1)
    # elif curr_angle == 4:
    #     curr_position = (x - 1, y)
    # elif curr_angle == 5:
    #     curr_position = (x - 1, y - 1)
    # elif curr_angle == 6:
    #     curr_position = (x, y - 1)
    # elif curr_angle == 7:
    #     curr_position = (x + 1, y - 1)
    
    curr_position = pos_from_angle(curr_position, curr_angle)
        

    # print(curr_angle)


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

def update_before():
    """
    Updates the map before the robot makes a turn. 
    """

    global curr_angle
    global curr_position

    prev_pos = curr_position
    
    prev_angle = curr_angle

    update_position()

    # print(prev_pos, curr_position)

    if (curr_position not in int_map):
        int_map[curr_position] = Intersection(curr_position[0], curr_position[1])

    int_map[curr_position].add_neighbor((prev_angle + 4) % 8, int_map[prev_pos])
    int_map[curr_position].add_neighbor((prev_angle + 5) % 8, Intersection.DNE)
    int_map[curr_position].add_neighbor((prev_angle + 3) % 8, Intersection.DNE)
    
    int_map[prev_pos].add_neighbor(prev_angle, int_map[curr_position])
    int_map[prev_pos].add_neighbor((prev_angle + 1) % 8, Intersection.DNE)
    int_map[prev_pos].add_neighbor((prev_angle - 1) % 8, Intersection.DNE)
    
    show_map()

    # if int_map[curr_position] == "UNKNOWN":
    #     int_map[curr_position].add_neighbor(curr_angle, "UNDRIVEN")

    # if int_map[tuple(prev_pos)] == "UNKNOWN":
    #     int_map[tuple(prev_pos)].add_neighbor(curr_angle, "UNDRIVEN")
    

def update_map(angle): 
    """
    Updates the map after a robot makes a turn
    """
    global curr_angle
    global curr_position

    prev_pos = curr_position
    prev_angle = curr_angle
    
    update_angle(angle)


    # print(prev_pos, curr_position)

    if not (angle is None):
        if angle > 0:
            for i in range(1, angle_to_header(abs(angle))):
                if int_map[curr_position].neighbors[(i + prev_angle) % 8] == Intersection.UNKNOWN:
                    int_map[curr_position].add_neighbor((i + prev_angle) % 8, Intersection.DNE)
                    # print(angle_to_header(angle))
        elif angle < 0:
            for i in range(1, angle_to_header(abs(angle))):
                if int_map[curr_position].neighbors[(prev_angle - i) % 8] == Intersection.UNKNOWN:
                    int_map[curr_position].add_neighbor((prev_angle - i) % 8, Intersection.DNE)
                    # print(angle_to_header(angle))
                
    
    # for key in int_map:
        # print(key, int_map[key])
        

def show_map():
    """
    Creates map based on intersection map
    """
    plt.clf()
    
    for coord in int_map:
        for i in range(8):
            neighbor = int_map[coord].neighbors[i]
            if type(neighbor) == Intersection:
                plt.plot((coord[0], neighbor.x), (coord[1], neighbor.y), "k-")
                #print(coord, (neighbor.x, neighbor.y))
            elif neighbor == Intersection.DNE:
                new_pos = pos_from_angle(coord, i)
                plt.plot((coord[0], coord[0] + (new_pos[0] - coord[0]) / 10), (coord[1], coord[1] + (new_pos[1] - coord[1]) / 10), "r:")
            
    
    plt.savefig("maps/curr_map1.png")

def pickling():
    global filename 
    global int_map

    print("Saving map to %s..." % filename)
    with open(filename, 'wb') as file:
        pickle.dump(int_map, file)

def loading_map():
    global filename
    global int_map

    print("Loading map from %s..." % filename)
    with open(filename, 'rb') as file:
        int_map = pickle.load(file)
        
def show_dijkstra():
    """
    Doesn't work yet... needs debugging
    """

    plt.clf()
    
    for coord in int_map:
       inter = int_map[coord]
       if inter.dir is not None:
           new_pos = pos_from_angle(coord, inter.dir)
           plt.arrow(coord[0], (new_pos[0] - coord[0]), coord[1], (new_pos[1] - coord[1]))
    
    plt.savefig("maps/dijsktra_map1.png")        
        
def euclid(pos1, pos2):
    return math.sqrt(((pos2[0]-pos1[0])**2) + ((pos2[1]-pos1[1])**2))
        
def dijkstra(goal):

    reset_costs()

    global int_map

    int_map[goal].cost = 0
    
    onDeck = [int_map[goal]]
    
    while len(onDeck) > 0:
        curr_node = onDeck.pop(0)
        pot_cost = curr_node.cost
        for i in range(8):
            neighbor = curr_node.neighbors[i]
            
            if type(neighbor) == Intersection:
                pot_cost += euclid((curr_node.x, curr_node.y), (neighbor.x, neighbor.y))

                if pot_cost < neighbor.cost:
                    if neighbor.cost < float('inf'):
                        onDeck.remove(neighbor)
                        
                    neighbor.cost = pot_cost
                    neighbor.dir = int ((i + 4) % 8)
                    bisect.insort(onDeck, neighbor)
                
def intersection_dijkstra(drive, sensor):
    """
    Called once a robot detects an intersection. Returns the angle turned. 
    (right is negative, left is positive, staraight is 0)
    """

    global prev_filter
    global prev_dir_filter
    global curr_position
    global curr_angle
    global time_error
    

    drive.drive("STRAIGHT", RIGHT)
    time.sleep(sleep_time)
    drive.drive("STOP", RIGHT)
    
    update_position()
    
    curr_intersection = int_map[curr_position]

    reading = sensor.read_line()
    
    prev_time = time.time()

    # Previous raw sensor reading
    reading = (0, 0, 0)

    # X_n-1
    prev_filter = (1,1,1)
    
    flag = 0
    curr_mid = 1
    index = -1

    direction = -2
    raw_reading = sensor.read_line()
    
    # print("curr_angle: ", curr_angle)
    
    dir_list = [(curr_angle + 1) % 8, (curr_angle + 2) % 8, (curr_angle + 3) % 8, (curr_angle + 4) % 8]
    
    if curr_intersection.dir == curr_angle:
        direction = 0
    elif curr_intersection.dir in dir_list:
        direction = 1
    else:
        direction = -1
        
        
    # turning
    if direction < 0: 
        drive.drive("SPIN", RIGHT)
        index = 2

    elif direction == 0:
        return 0

    else: 
        drive.drive("SPIN", LEFT)
        index = 0

    #variables for turning 
    first_time = -1.0
    turn_time = -1.0

    angle_dt = 0.0
    angle = 0.0
    angular_speed = 0.0

    detect_dir = 1
    begining_time = time.time()

    # turning filter
    while(True):
        curr_time = time.time()
        dt = curr_time - prev_time

        # current raw sensor reading
        raw_reading = sensor.read_line()
        pure_filter = detect(prev_filter, dt, raw_reading, turn_T)
        
        reading = threshold(pure_filter, reading, upper_threshold_turn, lower_threshold_turn)
        
        if reading[1] == 0 and flag != 1:
            flag = 1
        
        if flag == 1 and reading[1] == 1 and first_time != -1:
            curr_time = time.time()
            # drive.drive("STOP", LEFT)
            # input("enter")
            turn_time = curr_time - begining_time
            angle_dt = curr_time - first_time

            angle_dt = max(0.08, angle_dt)

            angle = bot_angle
            angular_speed = angle/angle_dt

            raw_ret = (angular_speed * turn_time *57.2957795) 
            
            ret = int((raw_ret + 22.5) / 45) * 45

            # print(raw_ret, ret)
            
            turned = angle_to_header(ret)
                

            # if angles are close to borderline, use timing to tie break
            if not (abs(ret) == 45):
                if abs(raw_ret) - abs(ret) > time_error:

                    # print("ADJUSTING ANGLE + ")

                    angle2 = abs(ret) + 45

                    
                    # print(f"ret: {ret}, angle2: {angle2}")

                    t1 = abs(turn_time - angle_times[abs(ret)])
                    # print(f"t1: {t1}")
                    t2 = abs(turn_time - angle_times[abs(angle2)])
                    # print(f"t2: {t2}")

                    if (t1 > t2):
                        ret = angle2

                elif abs(raw_ret) - abs(ret) < -(time_error):

                    # print("ADJUSTING ANGLE - ")

                    angle2 = abs(ret) - 45

                    # print(f"ret: {ret}, angle2: {angle2}")

                    t1 = abs(turn_time - angle_times[abs(ret)])
                    # print(f"t1: {t1}")
                    t2 = abs(turn_time - angle_times[abs(angle2)])
                    # print(f"t2: {t2}")

                    if (t1 > t2):
                        ret = angle2
                        

            
            # print(ret)

            # print(angle_dt, angle, angular_speed, turn_time, (angular_speed * turn_time * 57.2957795))

            # print(angle_dt, angle, angular_speed, turn_time)

            # print(turn_time)

            prev_filter = (0, 1, 0)
            prev_dir_filter = 0

            if ret>180:
                ret = 180
            elif ret == 135:
                ret = 90
                angle2 = 180
                t1 = abs(turn_time - angle_times[abs(ret)])
                # print(f"t1: {t1}")
                t2 = abs(turn_time - angle_times[abs(angle2)])
                # print(f"t2: {t2}")

                if (t1 > t2):
                    ret = angle2

            # print(f"final angle: {direction*ret}")
            
            tm = turn_time
            t1 = first_time - begining_time
            
            print("raw: ", raw_ret, " tm: ", tm, " t1: ", t1, " tm - t1: ", tm - t1, " tm/(tm - t1): ", tm / (tm - t1), " ret: ", direction * ret)
            if ret == 135:
                raise Exception("135 angle detected")
            return direction*ret

        
        if flag == 1 and reading[index] == 1 and first_time == -1:
            first_time = time.time()
            # drive.drive("STOP", LEFT)
            # input("Enter")
            if direction < 0: 
                drive.drive("SPIN", RIGHT)
                index = 2
            else: 
                drive.drive("SPIN", LEFT)
                index = 0

        prev_filter = pure_filter
        prev_time = curr_time

          
def run_dijkstra(drive, sensor):

    """
    Main drive loop that gets sensor readings, updates map, and haldels intersections
    """
    
    loading_map()
    goal = eval(input("Input goal coords"))
    
    dijkstra(goal)
    
    show_dijkstra()
    
    prev_time = time.time()
    global curr_angle
    global curr_position

    # Previous raw sensor reading
    while(True):
        curr_time = time.time()
        dt = curr_time - prev_time

        # current raw sensor reading
        

        # vic_dance()
        # print(raw_reading)
        if line_following(drive, sensor, dt):

            angle = intersection_dijkstra(drive, sensor)

            # print(curr_position, curr_angle)

            update_angle(angle)

            if (curr_position == goal):
                break

            pure_filter = (0,0,0)

            drive.drive("STOP", 1)
            # curr_time = time.time()
        
        
        prev_time = curr_time

    vic_dance()
    
def run(drive, sensor):

    """
    Main drive loop that gets sensor readings, updates map, and haldels intersections
    """
    
    prev_time = time.time()
    global curr_angle
    global curr_position

    # initilaize our first intersection in our map
    int_map[(0, 0)] = Intersection(0, 0)

    # Previous raw sensor reading
    while(True):
        curr_time = time.time()
        dt = curr_time - prev_time

        # current raw sensor reading
        

        # vic_dance()
        # print(raw_reading)
        if line_following(drive, sensor, dt):

            angle = intersection_handle(drive, sensor)

            # print(curr_position, curr_angle)

            update_map(angle)

            if (map_complete()):
                pickling()
                loading_map()
                break

            pure_filter = (0,0,0)

            drive.drive("STOP", 1)

            show_map()
            # curr_time = time.time()
        
        
        prev_time = curr_time

    vic_dance()


if __name__=="__main__":
    io = pigpio.pi()
    if not io.connected:
        print("Unable to connection to pigpio daemon!")
        sys.exit(0)
    print("GPIO ready...")
    
    drive = Drive(PIN_MOTOR1_LEGA, PIN_MOTOR1_LEGB, PIN_MOTOR2_LEGA, PIN_MOTOR2_LEGB, io)
    sensor = LineSensor(PIN_IR_LEFT, PIN_IR_MIDDLE, PIN_IR_RIGHT, io)


    try:
        run_dijkstra(drive, sensor)
        # loading_map()
        # vic_dance()
        # test(drive, sensor)
        # run(drive, sensor)
            
    except BaseException as ex:
        # Report the error, then continue with the normal shutdown.
        print("Ending due to exception: %s" % repr(ex))
        traceback.print_exc()
        # Shutdown cleanly (stopping the motors, then stopping the io).
    finally:
        drive.drive("STOP", 1)
        io.stop()