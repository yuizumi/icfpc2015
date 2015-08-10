#! /bin/bash

for i in `seq 0 23` ; do 
    python ../yuizumi/json2cp.py < ../problems/problem_${i}.json > ./problem_${i}.txt
done
