from cell import Cell


class Unit:
    def __init__(self, dict):
        self.members = [Cell(member) for member in dict['members']]
        self.pivot = Cell(dict['pivot'])

    def move(self, right, down):
        for member in self.members:
            member.x += right(member)
            member.y += down

        self.pivot.x += right(self.pivot)
        self.pivot.y += down

    def rotate(self, direction):
        for i in xrange(len(self.members)):
            self.members[i] = self.rotate_cell(self.members[i], direction)

    def rotate_cell(self, cell, direction):
        dx = cell.x - self.pivot.x
        dy = cell.y - self.pivot.y
        x0_adjust = (cell.y % 2 - self.pivot.y % 2)
        ex0 = dx * 2.0 + x0_adjust
        ey0 = dy * (3 ** 0.5)
        ex1 = (ex0 - ey0 * (3 ** 0.5) * direction) / 2.0
        ey1 = (ex0 * (3 ** 0.5) * direction + ey0) / 2.0
        new_cell = Cell({'x': self.pivot.x, 'y': self.pivot.y})
        new_cell.y += int(round(ey1 / (3 ** 0.5)))
        x1_adjust = (new_cell.y % 2 - self.pivot.y % 2)
        new_cell.x += (int(round(ex1)) - x1_adjust) / 2
        return new_cell

    def centering(self, width):
        left, right = self.left_right()
        r = (width - (right - left + 1)) / 2 - left
        self.move(lambda c: r, 0)

    def left_right(self):
        l = 10000000000000000
        r = 0
        for member in self.members:
            l = min(l, member.x)
            r = max(r, member.x)
        return l, r

    def __repr__(self):
        return "\tPivot: {}\tMember:{}".format(
            self.pivot.__repr__(),
            [member.__repr__() for member in self.members]
        )


