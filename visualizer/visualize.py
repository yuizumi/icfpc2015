import os, sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import json
import Tkinter as Tk
from tetris import *
from model import Input, CellState

Color = {
    CellState.Empty: "#e7e3e0",
    CellState.Fill: "#FFE677",
    CellState.Locked: "#bbbbbb"
}

class Visualizer(object):
    def __init__(self, input):
        """
        :type input:Input
        """
        self.input = input
        self.width = input.width * (EDGE + EDGE_MARGIN) + MARGIN
        self.height = input.height * (EDGE + EDGE_MARGIN) + MARGIN
        self.cells = [["" for _ in xrange(input.width)] for _ in xrange(input.height)]


    def initial_state(self, canvas):
        """
        :type canvas:Tk.Canvas
        """
        for y in xrange(self.input.height):
            for x in xrange(self.input.width):
                left_margin = 0 if y % 2 == 0 else (MARGIN / 2)
                x_pos = x * (EDGE + EDGE_MARGIN) + left_margin
                y_pos = y * (EDGE + EDGE_MARGIN)
                self.cells[y][x] = canvas.create_rectangle(x_pos,
                                                           y_pos,
                                                           x_pos + EDGE,
                                                           y_pos + EDGE,
                                                           fill=Color[CellState.Empty],
                                                           outline="#FFFFFF")

    def fill(self, canvas):
        """
        :type canvas:Tk.Canvas
        """
        states = [[CellState.Empty for _ in xrange(input.width)] for _ in xrange(input.height)]
        for cell in self.input.filled:
            states[cell.y][cell.x] = CellState.Fill
        print self.input.filled

        for y in xrange(self.input.height):
            for x in xrange(self.input.width):
                canvas.itemconfig(self.cells[y][x], fill=Color[states[y][x]])




    def visualize(self, i):
        print self.input
        root = Tk.Tk()
        root.title("visualize{}".format(i))
        root.geometry("{}x{}".format(self.width, self.height))
        canvas = Tk.Canvas(root, width=self.width, height=self.height)
        self.initial_state(canvas)
        self.fill(canvas)
        canvas.pack()
        root.mainloop()


if __name__ == '__main__':
    # for i in xrange(23):
    i = 23
    with open('../problems/problem_{}.json'.format(i), 'r') as f:
        input = Input(json.load(f))

    visualizer = Visualizer(input)
    visualizer.visualize(i)
