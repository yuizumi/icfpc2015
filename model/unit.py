from cell import Cell


class Unit:
    def __init__(self, dict):
        self.members = [Cell(member) for member in dict['members']]
        self.pivot = Cell(dict['pivot'])

    def __repr__(self):
        return "\tPivot: {}\tMember:{}".format(
            self.pivot.__repr__(),
            [member.__repr__() for member in self.members]
        )


