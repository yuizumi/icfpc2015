import json
import os
import subprocess

BasePath = os.path.dirname(os.path.abspath(__file__))
tester = "quicktest.sh"
tester_path = os.path.join(BasePath, '../', 'yuizumi', tester)

phrase = "ph'nglui mglw'nafh cthulhu r'lyeh wgah'nagl fhtagn."

valids = []
for board in xrange(24):
    with open(os.path.join(BasePath, '../problems/problem_{}.json'.format(board)), 'r') as f:
        d = json.load(f)

    sourceSeedLength = len(d['sourceSeeds'])
    print "==board {}==".format(board)
    for seed in xrange(sourceSeedLength):
        command = [tester_path, str(board), str(seed), phrase]
        result = subprocess.call(command)
        print result
        if result == 0:
            print "Board: {}, Seed: {}".format(board, seed)
            valids.append((board, seed))

print "valids:", len(valids)
print valids
with open('all.json', 'w') as f:
    j = []
    for board, seed in valids:
        d = {
            "problemId": int(board),
            "seed": int(seed),
            "solution": phrase
        }
        j.append(d)
    f.write(json.dumps(j))
