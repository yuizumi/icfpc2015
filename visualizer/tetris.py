import enum

FrameHeight = 600
FrameMargin = 10

CanvasWidth = 600
OptionWidth = 300
FrameWidth = CanvasWidth + OptionWidth
EDGE = 20
EDGE_MARGIN = 1
MARGIN = 20

OPTION_AREA = 150

class Key(enum.Enum):
    W = "f"
    E = "j"
    SE = "n"
    SW = "v"
    ROTATE_RIGHT = "h"
    ROTATE_LEFT = "g"

CharaToOperation = {
    "p": Key.W,
    "'": Key.W,
    "!": Key.W,
    ".": Key.W,
    "0": Key.W,
    "3": Key.W,

    "b": Key.E,
    "c": Key.E,
    "e": Key.E,
    "f": Key.E,
    "y": Key.E,
    "2": Key.E,

    "a": Key.SW,
    "g": Key.SW,
    "h": Key.SW,
    "i": Key.SW,
    "j": Key.SW,
    "4": Key.SW,

    "l": Key.SE,
    "m": Key.SE,
    "n": Key.SE,
    "o": Key.SE,
    " ": Key.SE,
    "5": Key.SE,

    "d": Key.ROTATE_RIGHT,
    "q": Key.ROTATE_RIGHT,
    "r": Key.ROTATE_RIGHT,
    "v": Key.ROTATE_RIGHT,
    "z": Key.ROTATE_RIGHT,
    "1": Key.ROTATE_RIGHT,

    "k": Key.ROTATE_LEFT,
    "s": Key.ROTATE_LEFT,
    "t": Key.ROTATE_LEFT,
    "u": Key.ROTATE_LEFT,
    "w": Key.ROTATE_LEFT,
    "x": Key.ROTATE_LEFT,

    "\t": '',
    "\n": '',
    "\r": '',
}

