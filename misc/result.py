#!/usr/bin/env python

import sys
import json

for line in sys.stdin.readlines():
  power = {}
  score = {}
  for result in json.loads(line):
    if result['powerScore'] != 'null':
      power[result['solution']] = result['powerScore']
      score[result['solution']] = result['score']

  for key in power:
    print u'{0}\t{1}\t{2}'.format(key, power[key], score[key])
