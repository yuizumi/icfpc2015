import json
import sys

n = int(sys.stdin.readline())
output = []
for i in xrange(n):
    id, seed = sys.stdin.readline().strip().split()
    solution = sys.stdin.readline()[:-1]
    output.append(
        {'problemId': int(id), 'seed': int(seed), 'solution': solution})
print json.dumps(output)
