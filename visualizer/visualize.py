import os, sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import argparse
import json
import time
import Tkinter as Tk
from tetris import *
from model import Input, CellState

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
        self.width = input.width * (EDGE + EDGE_MARGIN) + MARGIN + OPTION_AREA
        self.height = input.height * (EDGE + EDGE_MARGIN) + MARGIN
        self.cells = [["" for _ in xrange(input.width)] for _ in xrange(input.height)]
        self.current_unit = None
        self.states =[]
        self.locked = []
        self.seed_i = 0

    def move_unit(self, right, down):
        self.current_unit.move(right, down)
        print self.current_unit.members
        self.fill()


    def keyup(self, e):
        print 'up', e.keysym
        if e.keysym == "f":
            self.move_unit(-1, 0)
        if e.keysym == "j":
            self.move_unit(1, 0)
        if e.keysym == "v":
            if self.current_unit.pivot.y % 2 == 0:
                self.move_unit(-1, 1)
            else:
                self.move_unit(0, 1)
        if e.keysym == "n":
            if self.current_unit.pivot.y % 2 == 0:
                self.move_unit(0, 1)
            else:
                self.move_unit(1, 1)


    def initial_state(self):
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

        self.states = [[CellState.Empty for _ in xrange(input.width)] for _ in xrange(input.height)]
        for cell in self.input.filled:
            self.states[cell.y][cell.x] = CellState.Fill

        for cell in self.current_unit.members:
            self.states[cell.y][cell.x] = CellState.Active

        for cell in self.locked:
            self.state[cell.y][cell.x] = CellState.Locked

        for y in xrange(self.input.height):
            for x in xrange(self.input.width):
                self.canvas.itemconfig(self.cells[y][x], fill=Color[self.states[y][x]], outline="#FFFFFF")

        pivot = self.current_unit.pivot
        self.canvas.itemconfig(self.cells[pivot.y][pivot.x], outline="#000000")


    def bind_key(self):
        """
        :type canvas:Tk.Canvas
        """
        self.canvas.bind("<KeyRelease>", self.keyup)

    def get_rands(self):
        r = self.input.sourceSeeds[self.seed_i]
        for i in xrange(input.sourceLength):
            yield (r >> 16) & 0x7FFF
            r = (r * 1103515245 + 12345) % (1 << 32)

    def game(self):
        rands = self.get_rands()
        self.current_unit = self.input.units[rands.next()]
        self.current_unit.centering(self.input.width)
        print "current_unit:", self.current_unit
        self.initial_state()
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

    i = 23
    with open('../problems/problem_{}.json'.format(i), 'r') as f:
        input = Input(json.load(f))

    visualizer = Visualizer(input)
    visualizer.visualize(i)
