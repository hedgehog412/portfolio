class Intersection():

    UNKNOWN = "UNKNOWN"
    DNE = "DNE"


    
    def __init__(self, x, y, prev_heading = -1, prev_intersection = None):
        self.cost = float('inf')
        self.dir = None
        self.x = x
        self.y = y
        
        self.neighbors = [Intersection.UNKNOWN] * 8
        if prev_heading != -1:
            self.add_neighbor((prev_heading + 4) % 8, prev_intersection)
            
        
        
    def add_neighbor(self, heading, state):
        if (self.neighbors[heading] != "UNKNOWN") and (self.neighbors[heading] != state):
            # print(self.neighbors[heading])
            # print(state)
            raise Exception("Je suis confuse OwO")
        self.neighbors[heading] = state

    def __str__(self):
        return str(self.neighbors)
    
    def __lt__(self, other):
        return self.cost < other.cost
        