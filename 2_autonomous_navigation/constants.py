# Pins for motors
PIN_MOTOR1_LEGA = 7
PIN_MOTOR1_LEGB = 8

PIN_MOTOR2_LEGA = 5
PIN_MOTOR2_LEGB = 6

# Define the IR pins.
PIN_IR_LEFT   = 14    
PIN_IR_MIDDLE = 15      
PIN_IR_RIGHT  = 18     

# Directional Constants
LEFT = 1
RIGHT = 0

# Positive 1 means a right turn, -1 means a left turn first
INIT_DIRECTION = -1

# Direction Filtering Time Constant
standard_T = 0.1 

# Angle Dection Time Constant
turn_T = 0.03

# Line Following Time Constant
peace_T = 0.18

# How long to go forward after detcting intersection before turning
sleep_time = 0.35

# Ratio between sensors / Radius of robot
bot_angle = 0.19

# Line following filter thresholds
upper_threshold = 0.6
lower_threshold = 0.18

# Turning filter thresholds
upper_threshold_turn = 0.8
lower_threshold_turn = 0.2

# Direction determination filter thresholds
upper_threshold_dir = 0.6
lower_threshold_dir = 0.1

# Time constants for angles
angle_times = {45: 0.4, 90: 0.7, 135: 1, 180: 1.2, 225: 1.8, 270: 2.1, 315: 2.5, 360: 3.0, 0: 0.0}
# angle_times = {45: 0.35, 90: 0.6, 135: 1.1, 180: 1.2, 225: 1.8, 270: 2.1, 315: 2.5, 360: 3.0, 0: 0.0}

# Map of intersections (our map)
int_map = {}

# Initalize starting position
curr_position = (0, 0)

# Initialize starting angle
curr_angle = 2

# Initialize sensor readings
reading = (0, 0, 0)

# Initialize direction for line following
dir_reading = 0

# Initialize the previous filtered sensor value for intersection detection
prev_filter = (0, 0, 0)

# Initialize the previous direction filter value
prev_dir_filter = 0

# Initialize the filtered direction for line following
dir_filter = 0

# How close the measured angle has to be to a real angle to not rely on the time of the turn
time_error = 6

d_map = None

# ~~~~~~~~~~~~~~~~ULTRASOUND~~~~~~~~~~~~~~~~~~~~~~#
# ultrasound pins
L_TRIG = 13
L_ECHO = 16
M_TRIG = 19
M_ECHO = 20
R_TRIG = 26
R_ECHO = 21

trig_sleep_time = 0.10

# blocked distances
# straight_distance = 0.55 # 0.60
# diag_distance = 0.75 # 0.80
straight_distance = 0.10
diag_distance = 0.10

err_distance = 0.10
