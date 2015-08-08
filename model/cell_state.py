import enum
class CellState(enum.IntEnum):
    Empty = 0,
    Fill = 1,
    Locked = 2,
    Active = 3
