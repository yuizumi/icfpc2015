import os, sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import Tkinter as Tk
from tetris import *

BgColor='#FFFFFF'
TextWidth='32'
class MainFrame(Tk.Tk):
    def __init__(self):
        Tk.Tk.__init__(self)
        self.configure(bg=BgColor)
        self.geometry("{}x{}".format(FrameWidth + FrameMargin * 2, FrameHeight + FrameMargin * 2))
        self.canvas = self.layout_canvas()
        self.option_area = self.layout_option_area()


    def layout_canvas(self):
        canvas = Tk.Canvas(self,
                          width=CanvasWidth,
                          height=FrameHeight,
                          background=BgColor)
        canvas.focus_set()
        canvas.grid(row=0, column=0, padx=FrameMargin, pady=FrameMargin)
        return canvas

    def layout_option_area(self):
        option_area = OptionArea(self)
        option_area.grid(row=0, column=1)
        return option_area

class OptionArea(Tk.Frame):
    def __init__(self, parent):
        Tk.Frame.__init__(self, parent)
        self.configure(bg=BgColor,
                       width=300,
                       heigh=300)
        self.score_label = self.add_score_label()
        self.turn_label = self.add_turn_label()
        self.stage_selector = self.add_stage_list()
        self.binary_box = self.add_binary_box()

    def add_score_label(self):
        label = Tk.Label(self, text=0)
        label.pack()
        return label

    def add_turn_label(self):
        e = Tk.Entry(self)
        e.pack()
        return e

    def update_turn(self, num):
        self.turn_label.delete(0, Tk.END)
        self.turn_label.insert(0, str(num))

    def add_stage_list(self):
        stages = range(24)
        variable = Tk.StringVar(self)
        variable.set("Stage")
        w = apply(Tk.OptionMenu, (self, variable) + tuple(stages))
        w.configure(width=TextWidth, bg=BgColor)

        # l = Tk.Label(self, text="Stage")
        # l.pack(side=Tk.LEFT)
        w.pack()


    def add_binary_box(self):
        label = Tk.Label(self, text="Binary",
                         width=TextWidth, anchor=Tk.W,
                         bg=BgColor)
        label.pack()
        box = Tk.Text(self, height=5, width=TextWidth)
        scroll = Tk.Scrollbar(self)
        scroll.config(command=box.yview)
        box.config(yscrollcommand=scroll.set)
        scroll.pack(side=Tk.RIGHT, fill=Tk.Y)
        box.pack(expand=Tk.YES, fill=Tk.BOTH)

        return box




if __name__ == '__main__':
    main = MainFrame()
    # main.pack()
    main.mainloop()