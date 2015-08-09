import bisect
import copy

from model import Input, CellState, Unit


class Board(object):
    def __init__(self, input, seed):
        """
        :type self.unit:Unit
        """
        self.score = 0
        self.width = input.width
        self.height = input.height
        self.states = [[CellState.Empty for _ in xrange(input.width)] for _ in xrange(input.height)]
        self.locked = []
        self.locus = set()
        self.filled = input.filled
        self.units = input.units
        self.sourceSeeds = input.sourceSeeds
        self.sourceLength = input.sourceLength
        self.rands = self.get_rands(seed)
        self.unit = copy.deepcopy(self.units[self.rands.next()])
        self.unit.centering(self.width)
        self.update_states()

    def is_valid_move(self, right, down):
        for cell in self.unit.members:
            if not (0 <= cell.x + right(cell) < self.width and 0 <= cell.y + down < self.height):
                return False
            if self.states[cell.y + down][cell.x + right(cell)] > CellState.Valid:
                return False
        return True

    def is_valid_rotate(self, direction):
        for cell in self.unit.members:
            new_cell = self.unit.rotate_cell(cell, direction)
            if not (0 <= new_cell.x < self.width and 0 <= new_cell.y < self.height):
                return False
            if self.states[new_cell.y][new_cell.x] > CellState.Valid:
                return False
        return True

    def lock_unit(self):
        self.locked += self.unit.members
        self.update_states()
        deleted = self.delete_line()
        self.score += deleted * 100 + len(self.unit.members)

        self.unit = copy.deepcopy(self.units[self.rands.next()])
        self.unit.centering(self.width)
        self.locus = set()

    def move_unit(self, right, down, callback):
        if self.is_valid_move(right, down):
            self.unit.move(right, down)
        else:
            self.lock_unit()
        self.update_states()
        callback()

    def rotate_unit(self, direction, callback):
        if self.is_valid_rotate(direction):
            self.unit.rotate(direction)
        else:
            self.lock_unit()
        self.update_states()
        callback()

    def initialize_states(self):
        for y in xrange(self.height):
            for x in xrange(self.width):
                self.states[y][x] = CellState.Empty

    def delete_line(self):
        deleted_lines = []
        for y in range(self.height)[::-1]:
            for x in xrange(self.width):
                if self.states[y][x] < CellState.LockOrFill:
                    break
            else:
                deleted_lines.append(y)

        deleted_lines.sort()
        for cell in self.locked + self.filled:
            if cell.y in deleted_lines:
                cell.x = -1

            d = len(deleted_lines) - bisect.bisect_left(deleted_lines, cell.y)
            cell.y += d
        self.locked = [cell for cell in self.locked if cell.x > -1]
        self.filled = [cell for cell in self.filled if cell.x > -1]
        return len(deleted_lines)

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


    def move_W(self, callback):
        self.move_unit(lambda c: -1, 0, callback)

    def move_E(self, callback):
        self.move_unit(lambda c: 1, 0, callback)

    def move_SW(self, callback):
        self.move_unit(lambda c: -1 if c.y % 2 == 0 else 0, 1, callback)

    def move_SE(self, callback):
        self.move_unit(lambda c: 0 if c.y % 2 == 0 else 1, 1, callback)

    def rotate_R(self, callback):
        self.rotate_unit(-1, callback)

    def rotate_L(self, callback):
        self.rotate_unit(+1, callback)