import json
import sys

lines = sys.stdin.readlines()
output = []
for i in xrange(0, len(lines), 2):
    id, seed = lines[i].strip().split()
    solution = lines[i + 1].strip()
    output.append(
        {'problemId': int(id), 'seed': int(seed), 'solution': solution})
print json.dumps(output)
