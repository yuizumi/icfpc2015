#!/usr/bin/env python

import sys
import json


def toBin(i):
  if i == None:
    return "None"
  i = int(i)
  result = []
  temp = abs(i)
  for j in range(0, 18):
    result.append(str(temp % 2))
    temp /= 2
  return ''.join(result)

for line in sys.stdin.readlines():
  for result in json.loads(line):
    if len(sys.argv) == 1 or result['tag'] == sys.argv[1]:
      score = result['score']
      result['powerScore'] = toBin(result['powerScore'])
      if score == None or int(score) > 0:
        print u'Problem {problemId} (seed {seed}): {score} points (contained: {powerScore})\n\t{solution}'.format(**result)

