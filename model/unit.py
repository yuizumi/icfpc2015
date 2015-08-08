from cell import Cell


class Unit:
    def __init__(self, dict):
        self.members = [Cell(member) for member in dict['members']]
        self.pivot = Cell(dict['pivot'])

    def move(self, right, down):
        for member in self.members:
            member.x += right
            member.y += down

        self.pivot.x += right
        self.pivot.y += down

    def centering(self, width):
        left, right = self.left_right()
        r = width / 2 - (right - left) / 2
        self.move(r, 0)

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


