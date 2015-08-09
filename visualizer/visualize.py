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


class GameSet():
    def __init__(self, board_id, seed, operations):
        self.board_id = board_id
        self.seed = seed
        self.operations = operations

    def __repr__(self):
        return "Board: {} Seed: {}\nOperations: {}".format(self.board_id, self.seed, self.operations)


class Visualizer(object):
    def __init__(self, game_sets):
        """
        :type input:Input
        """
        self.game_sets = game_sets
        self.game_index = -1
        self.edge = None
        self.gui = None
        self.binary = None
        self.score_label = None
        self.input = None
        self.board = None
        self.cells = None

    def keyup(self, e):
        """
        Key setting.
        """
        if e.keysym == Key.W.value:
            self.board.move_unit(lambda c: -1, 0, self.fill)
        elif e.keysym == Key.E.value:
            self.board.move_unit(lambda c: 1, 0, self.fill)
        elif e.keysym == Key.SW.value:
            self.board.move_unit(lambda c: -1 if c.y % 2 == 0 else 0,
                                 1, self.fill)
        elif e.keysym == Key.SE.value:
            self.board.move_unit(lambda c: 0 if c.y % 2 == 0 else 1,
                                 1, self.fill)

        elif e.keysym == 'space':
            self.next_game()

        if e.keysym == 'Escape':
            self.gui.destroy()

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


    def bind(self):
        self.gui.canvas.bind("<KeyRelease>", self.keyup)

    def next_game(self):
        """
        Finish current game and start next game.
        """
        self.game_index += 1
        if self.game_index >= len(game_sets):
            print "Your game is finished."
            return
        game = self.game_sets[self.game_index]
        print game

        # Import game file
        with open(os.path.join(BasePath, '../problems/problem_{}.json'.format(game.board_id)), 'r') as f:
            self.input = Input(json.load(f))

        self.board = Board(self.input, game.seed)
        edge = min(CanvasWidth / self.input.width,
                   FrameHeight / self.input.height) - EDGE_MARGIN

        self.edge = edge - (edge % 2)
        self.initialize_cells()
        self.gui.update()
        self.fill()
        self.gui.mainloop()

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

    args = parser.parse_args()
    game_sets = []

    if args.is_std:
        print "stdinput"
    else:
        game_sets.append(GameSet(args.board_id, args.seed, []))

    visualizer = Visualizer(game_sets)
    visualizer.visualize()
