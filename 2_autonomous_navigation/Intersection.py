class Intersection():

    UNKNOWN = "UNKNOWN"
    DNE = "DNE"
    UNDRIVEN = "UNDRIVEN"

    def __init__(self, x, y, prev_heading = -1, prev_intersection = None):
        self.cost = float('inf')
        self.dir = None
        self.x = x
        self.y = y
        
        self.neighbors = [Intersection.UNKNOWN] * 8
        self.neighbors_blocked = [False] * 8

        if prev_heading != -1:
            self.add_neighbor((prev_heading + 4) % 8, prev_intersection)
        
    def add_neighbor(self, heading, state):
        # If we are overwriting anything other than unknown with something different from what's in the map, there's an error. 
        # Second condition: If it is currently undriven, we can overwrite it with ONLY an intersection
        if ((self.neighbors[heading] != "UNKNOWN") and (self.neighbors[heading] != state)) and not \
            (state == "UNDRIVEN" and type(self.neighbors[heading]) == Intersection) and not \
                (self.neighbors[heading] == "UNDRIVEN" and type(state) == Intersection):
            print("neighbor heading = ", self.neighbors[heading])
            print("state = ", state)
            raise Exception("Je suis confuse OwO")
        elif state == "UNDRIVEN":
            self.neighbors_blocked[heading] = True
            if self.neighbors[heading] == "UNKNOWN":
                self.neighbors[heading] = state
        else: 
            self.neighbors[heading] = state

    def __str__(self):
        return str(self.neighbors)
    
    def __lt__(self, other):
        return self.cost < other.cost

    def map_complete(int_map):
        """
        Returns true if the map is complete, false otherwise
        """
        for v in int_map.values():
            for neighbor in v.neighbors:
                if neighbor == Intersection.UNKNOWN:       
                    return False
        return True
    
    def clear_blocks(int_map):
        """
        Clears all blocks on streets
        """
        for v in int_map.values():
            v.neighbors_blocked = [False] * 8
            for i in range(8):
                neighbor = v.neighbors[i]
                if neighbor == Intersection.UNDRIVEN:       
                    v.neighbors[i] = Intersection.UNKNOWN
            
