#!/usr/bin/env python

import sys
import json

for line in sys.stdin.readlines():
  dict = {}
  for result in json.loads(line):
    if result['powerScore'] != 'null':
      dict[result['solution']] = result['powerScore']

  for key in dict:
    print u'{0}\t{1}'.format(key, dict[key])
