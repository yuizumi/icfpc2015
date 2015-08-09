import os, sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import argparse
import json
import time
import Tkinter as Tk
from tetris import *
from model import Input, CellState, Board
from gui import MainFrame

Color = {
    CellState.Empty: "#e7e3e0",
    CellState.Fill: "#FFE677",
    CellState.Active: "#44C2B3",
    CellState.Locked: "#bbbbbb",
}

BgColor = "#57B196"

class Visualizer(object):
    def __init__(self):
        """
        :type input:Input
        """
        self.edge = None
        self.gui = None
        self.binary = None
        self.score_label = None
        self.input = None
        self.board = None
        self.cells = None
        # self.boards = boards

    def keyup(self, e):
        if e.keysym == Key.W.value:
            self.board.move_unit(lambda c: -1, 0, self.fill)
        if e.keysym == Key.E.value:
            self.board.move_unit(lambda c: 1, 0, self.fill)
        if e.keysym == Key.SW.value:
            self.board.move_unit(lambda c: -1 if c.y % 2 == 0 else 0,
                                 1, self.fill)
        if e.keysym == Key.SE.value:
            self.board.move_unit(lambda c: 0 if c.y % 2 == 0 else 1,
                                 1, self.fill)


    def initialize_cells(self):
        """
        :type canvas:Tk.Canvas
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
        :type canvas:Tk.Canvas
        """
        for y in xrange(self.input.height):
            for x in xrange(self.input.width):
                self.gui.canvas.itemconfig(self.cells[y][x], fill=Color[self.board.states[y][x]], outline="#FFFFFF")

        pivot = self.board.unit.pivot
        self.gui.canvas.itemconfig(self.cells[pivot.y][pivot.x], outline="#000000")
        self.gui.option_area.score_label.configure(text=self.board.score)


    def bind(self):
        """
        :type canvas:Tk.Canvas
        """
        self.gui.canvas.bind("<KeyRelease>", self.keyup)

    def game(self, board_id, seed):
        print "Game start!"
        with open('../problems/problem_{}.json'.format(board_id), 'r') as f:
            self.input = Input(json.load(f))

        self.gui.canvas.create_rectangle(0,0,10,10)

        self.board = Board(self.input)
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
        self.game(0, 0)
        self.gui.mainloop()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Visualizer')
    parser.add_argument('--std', dest='is_std', action='store_true', help='Receive std input.')
    parser.add_argument('--id', dest='board_id', default=0, type=int, help='Board id.')
    parser.add_argument('--seed', dest='seed', default=0, type=int, help='Seed index.')

    args = parser.parse_args()
    visualizer = Visualizer()
    visualizer.visualize()

    if args.is_std:
        print "stdinput"
    else:
        visualizer.game(args.board_id, args.seed)

