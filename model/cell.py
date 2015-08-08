class Cell:
    def __init__(self, dict):
        self.x = int(dict['x'])
        self.y = int(dict['y'])

    def __str__(self):
        return "(" + str(self.x) + ", " + str(self.y) + ")"

    def __repr__(self):
        return "[{}, {}]".format(self.x, self.y)
