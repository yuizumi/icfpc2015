#! /bin/bash

for i in `seq 0 23` ; do 
    python ../yuizumi/json2cp.py < ../problems/problem_${i}.json | ./greedy | ./translate | python ../yuizumi/out2json.py 
done
