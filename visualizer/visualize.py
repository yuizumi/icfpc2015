import os, sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import argparse
import json
import time
import Tkinter as Tk
from tetris import *
from model import Input, CellState, Board

Color = {
    CellState.Empty: "#e7e3e0",
    CellState.Fill: "#FFE677",
    CellState.Active: "#ffad3c",
    CellState.Locked: "#bbbbbb"
}


class Visualizer(object):
    def __init__(self, input):
        """
        :type input:Input
        """
        self.input = input
        self.canvas = None
        self.board = None
        self.width = input.width * (EDGE + EDGE_MARGIN) + MARGIN + OPTION_AREA
        self.height = input.height * (EDGE + EDGE_MARGIN) + MARGIN
        self.cells = [["" for _ in xrange(input.width)] for _ in xrange(input.height)]


    def keyup(self, e):
        print 'up', e.keysym
        if e.keysym == Key.W.value:
            self.board.move_unit(lambda c: -1, 0, self.fill)
        if e.keysym == Key.E.value:
            self.board.move_unit(lambda c: 1, 0, self.fill)
        if e.keysym == Key.SW.value:
            self.board.move_unit(lambda c: -1 if c.y % 2 == 0 else 0,
                                 1, self.fill)
        if e.keysym == Key.SE.value:
            self.board.move_unit(lambda c:0 if c.y % 2 == 0 else 1,
                                 1, self.fill)


    def initialize_cells(self):
        """
        :type canvas:Tk.Canvas
        """
        for y in xrange(self.input.height):
            for x in xrange(self.input.width):
                left_margin = 0 if y % 2 == 0 else (MARGIN / 2)
                x_pos = x * (EDGE + EDGE_MARGIN) + left_margin
                y_pos = y * (EDGE + EDGE_MARGIN)
                self.cells[y][x] = self.canvas.create_rectangle(x_pos,
                                                                y_pos,
                                                                x_pos + EDGE,
                                                                y_pos + EDGE,
                                                                fill=Color[CellState.Empty],
                                                                outline="#FFFFFF")

    def fill(self):
        """
        :type canvas:Tk.Canvas
        """
        for y in xrange(self.input.height):
            for x in xrange(self.input.width):
                self.canvas.itemconfig(self.cells[y][x], fill=Color[self.board.states[y][x]], outline="#FFFFFF")

        pivot = self.board.unit.pivot
        self.canvas.itemconfig(self.cells[pivot.y][pivot.x], outline="#000000")


    def bind_key(self):
        """
        :type canvas:Tk.Canvas
        """
        self.canvas.bind("<KeyRelease>", self.keyup)

    def game(self):
        self.initialize_cells()
        self.board = Board(self.input)
        self.fill()


    def visualize(self, i):
        root = Tk.Tk()
        root.title("visualize{}".format(i))
        root.geometry("{}x{}".format(self.width, self.height))

        self.canvas = Tk.Canvas(root, width=self.width, height=self.height)
        self.canvas.focus_set()
        self.bind_key()

        self.game()

        self.canvas.pack()
        root.mainloop()


if __name__ == '__main__':
    i = 2
    with open('../problems/problem_{}.json'.format(i), 'r') as f:
        input = Input(json.load(f))

    visualizer = Visualizer(input)
    visualizer.visualize(i)
