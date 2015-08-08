import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import json
import Tkinter as Tk
from tetris import *
from model import Input, CellState


class Visualizer(object):
    def __init__(self, input):
        """
        :type input:Input
        """
        self.input = input
        self.width = input.width * (EDGE + EDGE_MARGIN) + MARGIN
        self.height = input.height * (EDGE + EDGE_MARGIN) + MARGIN


    def initial_state(self, canvas):
        """
        :type canvas:Tk.Canvas
        """
        for y in xrange(self.input.height):
            for x in xrange(self.input.width):
                left_margin = 0 if y % 2 == 0 else (MARGIN / 2)
                canvas.create_rectangle(x * (EDGE + EDGE_MARGIN),
                                        y * (EDGE + EDGE_MARGIN),
                                        EDGE,
                                        EDGE,
                                        fill="#FFE677",
                                        outline="#FFFFFF")


    def visualize(self, i):
        print self.input
        root = Tk.Tk()
        root.title("visualize")
        root.geometry("{}x{}".format(self.width, self.height))
        canvas = Tk.Canvas(root, width=self.width, height=self.height)
        self.initial_state(canvas)
        canvas.pack()
        # canvas.place(x=0, y=0)
        root.mainloop()



if __name__ == '__main__':
    with open('../problems/problem_{}.json'.format(2), 'r') as f:
        input = Input(json.load(f))

    visualizer = Visualizer(input)
    visualizer.visualize(2)
