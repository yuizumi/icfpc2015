import os, sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import argparse
import json
import time
import Tkinter as Tk
from tetris import *
from model import Input, CellState, Board
from gui import MainFrame

BasePath = os.path.dirname(os.path.abspath(__file__))

Color = {
    CellState.Empty: "#e7e3e0",
    CellState.Fill: "#FFE677",
    CellState.Active: "#44C2B3",
    CellState.Locked: "#bbbbbb",
}

BgColor = "#57B196"

Operators = ['new', Key.E, Key.W, Key.SE, Key.SW, Key.ROTATE_RIGHT, Key.ROTATE_LEFT]
OperatorToInt = {
    Key.E: 1,
    Key.W: 2,
    Key.SE: 3,
    Key.SW: 4,
    Key.ROTATE_RIGHT: 5,
    Key.ROTATE_LEFT: 6
}

class GameSet():
    def __init__(self, board_id, seed, operations):
        self.board_id = board_id
        self.seed = seed
        self.operations = operations

    def __repr__(self):
        return "Board: {} Seed: {}\nOperations: {}".format(self.board_id, self.seed, self.operations)

class KeyEvent():
    def __init__(self, keysym):
        self.keysym = keysym

class Visualizer(object):
    def __init__(self, game_sets, auto=False, is_keyword=False):
        """
        :type input:Input
        """
        self.game = None
        self.game_sets = game_sets
        self.game_index = 0
        self.operation_index = 0
        self.edge = None
        self.gui = None
        self.binary = None
        self.score_label = None
        self.input = None
        self.board = None
        self.cells = None
        self.auto = auto
        self.is_keyword = is_keyword
        self.operations = []
        self.turn_num = 0

    def update_board(self, key):
        self.operations.append(OperatorToInt[key])
        self.turn_num += 1
        self.gui.option_area.update_turn(self.turn_num)

    def keyup(self, e):
        """
        Key setting.
        """
        if e.keysym == Key.W.value:
            self.update_board(Key.W)
            self.board.move_W(self.fill)
        elif e.keysym == Key.E.value:
            self.update_board(Key.E)
            self.board.move_E(self.fill)
        elif e.keysym == Key.SW.value:
            self.update_board(Key.SW)
            self.board.move_SW(self.fill)
        elif e.keysym == Key.SE.value:
            self.update_board(Key.SE)
            self.board.move_SE(self.fill)
        elif e.keysym == Key.ROTATE_RIGHT.value:
            self.update_board(Key.ROTATE_RIGHT)
            self.board.rotate_R(self.fill)
        elif e.keysym == Key.ROTATE_LEFT.value:
            self.update_board(Key.ROTATE_LEFT)
            self.board.rotate_L(self.fill)
        elif e.keysym == 'space':
            self.next_game()
        elif e.keysym == 'Escape':
            self.gui.destroy()
        elif e.keysym == 'Return':
            if self.auto != -1 and self.operation_index < len(self.game.operations):
                self.gui.after(i * self.auto, self.keyup, e)
            self.next_step()

        # else:
        #     print e.keysym


    def initialize_cells(self):
        """
        Initialize all cells. This method should be called when starting new game.
        """
        self.gui.canvas.delete("all")
        self.cells = [["" for _ in xrange(self.input.width)] for _ in xrange(self.input.height)]
        for y in xrange(self.input.height):
            for x in xrange(self.input.width):
                left_margin = 0 if y % 2 == 0 else (self.edge / 2)
                x_pos = x * (self.edge + EDGE_MARGIN) + left_margin
                y_pos = y * (self.edge + EDGE_MARGIN)
                self.cells[y][x] = self.gui.canvas.create_rectangle(x_pos,
                                                                    y_pos,
                                                                    x_pos + self.edge,
                                                                    y_pos + self.edge,
                                                                    fill=Color[CellState.Empty],
                                                                    outline="#FFFFFF")

    def fill(self):
        """
        Redraw cells.
        """
        for y in xrange(self.input.height):
            for x in xrange(self.input.width):
                self.gui.canvas.itemconfig(self.cells[y][x], fill=Color[self.board.states[y][x]], outline="#FFFFFF")

        pivot = self.board.unit.pivot
        self.gui.canvas.itemconfig(self.cells[pivot.y][pivot.x], outline="#000000")
        self.gui.option_area.score_label.configure(text=self.board.score)
        if self.operations:
            self.gui.option_area.binary_box.insert(Tk.END, self.operations[-1])


    def bind(self):
        self.gui.canvas.bind("<KeyRelease>", self.keyup)

    def next_step(self):
        if self.operation_index >= len(self.game.operations):
            print "Finish this game."
            return
        o = self.game.operations[self.operation_index]
        if self.is_keyword:
            e = CharaToOperation[o.lower()].value
        else:
            e = Operators[int(o)].value
        self.keyup(KeyEvent(e))
        self.operation_index += 1

    def next_game(self):
        """
        Finish current game and start next game.
        """
        self.operation_index = 0
        if self.game_index >= len(game_sets):
            print "Your game is finished."
            return

        self.game = self.game_sets[self.game_index]
        print "Start new game:", self.game

        # Import game file
        with open(os.path.join(BasePath, '../problems/problem_{}.json'.format(self.game.board_id)), 'r') as f:
            self.input = Input(json.load(f))

        self.board = Board(self.input, self.game.seed)
        edge = min(CanvasWidth / self.input.width,
                   FrameHeight / self.input.height) - EDGE_MARGIN

        self.edge = edge - (edge % 2)
        self.initialize_cells()
        self.fill()
        self.game_index += 1

    def visualize(self):
        self.gui = MainFrame()
        self.bind()
        self.next_game()
        self.gui.mainloop()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Visualizer')
    parser.add_argument('--std', dest='is_std', action='store_true', help='Receive std input.')
    parser.add_argument('--id', dest='board_id', default=0, type=int, help='Board id.')
    parser.add_argument('--seed', dest='seed', default=0, type=int, help='Seed index.')
    parser.add_argument('--auto', dest='auto', default=-1, type=int, help='Auto play mode.')
    parser.add_argument('--keyword', dest='is_keyword', action='store_true', help='')

    args = parser.parse_args()
    game_sets = []

    if args.is_std:
        n = int(raw_input())
        for i in xrange(n):
            id, seed = map(int, raw_input().split())
            operations = raw_input()
            game_sets.append(GameSet(id, seed, operations))

    else:
        game_sets.append(GameSet(args.board_id, args.seed, ""))

    visualizer = Visualizer(game_sets, args.auto, args.is_keyword)
    visualizer.visualize()
