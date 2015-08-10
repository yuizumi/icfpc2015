import json
import os
import subprocess

BasePath = os.path.dirname(os.path.abspath(__file__))
tester = "quicktest.sh"
tester_path = os.path.join(BasePath, '../', 'yuizumi', tester)

phrases = [
    "ei!Azathoth"
]
tag = "coco3"

def check(phrase):
    for board in xrange(24):
        with open(os.path.join(BasePath, '../problems/problem_{}.json'.format(board)), 'r') as f:
            d = json.load(f)

        sourceSeedLength = len(d['sourceSeeds'])
        print "==board {}==".format(board)
        for seed in xrange(sourceSeedLength):
            command = [tester_path, str(board), str(seed), "  " + phrase]
            result = subprocess.call(command)
            print result
            if result == 0:
                # print "Board: {}, Seed: {}".format(board, seed)
                return tuple([board, seed, phrase])
    return None



valids = []
for phrase in phrases:
    valid = check(phrase)
    if valid:
        valids.append(valid)

print "valids:", len(valids)
print valids
with open('all.json', 'w') as f:
    j = []
    for board, seed, phrase in valids:
        d = {
            "problemId": int(board),
            "seed": int(seed),
            "solution": phrase,
            "tag": tag
        }
        j.append(d)
    f.write(json.dumps(j))
