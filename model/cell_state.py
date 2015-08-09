import enum
class CellState(enum.IntEnum):
    Empty = 0
    Active = 1
    Valid = 2
    LockOrFill = 3
    Fill = 4
    Locked = 5
