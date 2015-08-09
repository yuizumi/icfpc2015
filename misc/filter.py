#!/usr/bin/env python

import sys
import json

for line in sys.stdin.readlines():
  for result in json.loads(line):
    if result['tag'] == sys.argv[1] or len(sys.argv) == 1:
      print u'Problem {problemId} (seed {seed}): {score} points ({powerScore} phrases)\n\t{solution}'.format(**result)

