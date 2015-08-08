import Cell

class Unit:
    def __init__(self, dict):
        self.members = [Cell(member) for member in dict['members']]
        self.pivot   = Cell(dict['pivot'])

