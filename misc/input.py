#!/usr/bin/env python

import sys
import json

class Cell:
  def __init__(self, dict):
    self.x = int(dict['x'])
    self.y = int(dict['y'])

  def __str__(self):
    return "(" + str(self.x) + ", " + str(self.y) + ")"

class Unit:
  def __init__(self, dict):
    self.members = [Cell(member) for member in dict['members']]
    self.pivot   = Cell(dict['pivot'])

class Input:
  def __init__(self, dict):
    self.id           = int(dict['id'])
    self.units        = [Unit(unit) for unit in dict['units']]
    self.width        = int(dict['width'])
    self.height       = int(dict['height'])
    self.filled       = [Cell(cell) for cell in dict['filled']]
    self.sourceLength = int(dict['sourceLength'])
    self.sourceSeeds  = [int(seed) for seed in dict['sourceSeeds']]

for line in sys.stdin.readlines():
  input = Input(json.loads(line))

  print "problem id = {0}, size = {1} * {2}, length = {3}".format(input.id, input.width, input.height, input.sourceLength)
  print "seeds"
  for seed in input.sourceSeeds:
    print "\t{0}".format(seed)
  print "units"
  for unit in input.units:
    print "\tpivot = {0}, {1}".format(unit.pivot, [str(member) for member in unit.members])
  print "filled"
  for cell in input.filled:
    print "\t{0}".format(cell)
