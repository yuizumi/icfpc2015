"""
Converts JSON to a competitive-programming format."""

import argparse
import json
import os

BasePath = os.path.dirname(os.path.abspath(__file__))
def p(board, seed, is_all):
    with open(os.path.join('../../../problems', 'problem_{}.json'.format(board)), 'r') as f:
        spec = json.load(f)
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
        if is_all:
            print len(spec['sourceSeeds'])
            print ' '.join(str(seed) for seed in spec['sourceSeeds'])
        else:
            print 1
            print spec['sourceSeeds'][seed]

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Command: make && python  json2cppy.py  --id 2 --seed 0 | ./basic')
    parser.add_argument('--id', dest='board_id', default=0, type=int, help='Board id.')
    parser.add_argument('--seed', dest='seed', default=0, type=int, help='Seed index.')
    parser.add_argument('--all', dest='is_all', action='store_true', help='All seed.')

    args = parser.parse_args()
    p(args.board_id, args.seed, args.is_all)