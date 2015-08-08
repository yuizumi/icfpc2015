from cell import Cell
from unit import Unit

class Input:
    def __init__(self, dict):
        self.id           = int(dict['id'])
        self.units        = [Unit(unit) for unit in dict['units']]
        self.width        = int(dict['width'])
        self.height       = int(dict['height'])
        self.filled       = [Cell(cell) for cell in dict['filled']]
        self.sourceLength = int(dict['sourceLength'])
        self.sourceSeeds  = [int(seed) for seed in dict['sourceSeeds']]
