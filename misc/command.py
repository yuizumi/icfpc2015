#!/usr/bin/env python

import sys
import json

dict = {
  'p': 'move W', '\'': 'move W', '!': 'move W', '.': 'move W', '0': 'move W', '3': 'move W',
  'b': 'move E', 'c' : 'move E', 'e': 'move E', 'f': 'move E', 'y': 'move E', '2': 'move E',
  'a': 'move SW', 'g': 'move SW', 'h': 'move SW', 'i': 'move SW', 'j': 'move SW', '4': 'move SW',
  'l': 'move SE', 'm': 'move SE', 'n': 'move SE', 'o': 'move SE', ' ': 'move SE', '5': 'move SE',
  'd': 'rotate clockwise', 'q': 'rotate clockwise', 'r': 'rotate clockwise', 'v': 'rotate clockwise', 'z': 'rotate clockwise', '1': 'rotate clockwise',
  'k': 'rotate counter-clockwise', 's': 'rotate counter-clockwise', 't': 'rotate counter-clockwise', 'u': 'rotate counter-clockwise', 'w': 'rotate counter-clockwise', 'x': 'rotate counter-clockwise',
  '\t': '', '\n': '', '\r': ''
}

for line in sys.stdin.readlines():
  chars = list(line.lower())
  for char in chars:
    print dict[char]
