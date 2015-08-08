import copy
from model import Input, CellState, Unit


class Board(object):
    def __init__(self, input):
        """
        :type self.unit:Unit
        """
        self.width = input.width
        self.height = input.height
        self.states = [[CellState.Empty for _ in xrange(input.width)] for _ in xrange(input.height)]
        self.locked = []
        self.filled = input.filled
        self.units = input.units
        self.sourceSeeds = input.sourceSeeds
        self.sourceLength = input.sourceLength
        self.rands = self.get_rands(0)
        self.unit = copy.deepcopy(self.units[self.rands.next()])
        self.unit.centering(self.width)

        self.update_states()

    def is_valid_move(self, right, down):
        for cell in self.unit.members:
            print cell
            if not (0 <= cell.x + right(cell) < self.width and 0 <= cell.y + down < self.height):
                print "Wall"
                return False
            print cell.x + right(cell), cell.y + down, self.states[cell.y + down][cell.x + right(cell)]
            if self.states[cell.y + down][cell.x + right(cell)] > CellState.Valid:
                print "Filled"
                return False
        return True

    def print_states(self):
        for y in xrange(self.height):
            for x in xrange(self.width):
                print self.states[y][x],
            print

    def lock_unit(self):
        self.locked += self.unit.members
        print self.locked
        self.unit = copy.deepcopy(self.units[self.rands.next()])
        self.unit.centering(self.width)

    def move_unit(self, right, down, callback):
        self.print_states()
        if self.is_valid_move(right, down):
            self.unit.move(right, down)
        else:
            self.lock_unit()
        self.update_states()
        callback()

    def initialize_states(self):
        for y in xrange(self.height):
            for x in xrange(self.width):
                self.states[y][x] = CellState.Empty

    def update_states(self):
        self.initialize_states()
        for cell in self.filled:
            self.states[cell.y][cell.x] = CellState.Fill

        for cell in self.unit.members:
            self.states[cell.y][cell.x] = CellState.Active

        for cell in self.locked:
            self.states[cell.y][cell.x] = CellState.Locked


    def apply_to_all(self, func):
        pass

    def get_rands(self, i):
        r = self.sourceSeeds[i]
        for i in xrange(self.sourceLength):
            yield ((r >> 16) & 0x7FFF) % len(self.units)
            r = (r * 1103515245 + 12345) % (1 << 32)

