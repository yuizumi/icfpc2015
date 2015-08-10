#!/bin/bash

prob_dir=${0%/*}/../problems
( python json2cp.py < ${prob_dir}/problem_$1.json
  echo 1 ; echo $1 $2 ; echo "$3"
) | ${0%/*}/validate
