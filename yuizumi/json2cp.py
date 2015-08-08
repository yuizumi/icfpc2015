"""Converts JSON to a competitive-programming format."""

import json
import sys

spec = json.load(sys.stdin)
print spec['id']
print len(spec['units'])
for unit in spec['units']:
    print len(unit['members'])
    print ' '.join('{x} {y}'.format(**m) for m in unit['members'])
    print '{x} {y}'.format(**unit['pivot'])
print '{width} {height}'.format(**spec)
print len(spec['filled'])
print ' '.join('{x} {y}'.format(**c) for c in spec['filled'])
print spec['sourceLength']
print len(spec['sourceSeeds'])
print ' '.join(str(seed) for seed in spec['sourceSeeds'])
