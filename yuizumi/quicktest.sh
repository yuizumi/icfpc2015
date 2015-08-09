#!/bin/bash

prob_dir=${0%/*}/../problems
(
    python json2cp.py < ${prob_dir}/problem_$1.json
    echo 1
    echo "$1 $2"
    echo "$3" | tr A-Z a-z \
              | tr "p'\!.03bcefy2aghij4lmno 5dqrvz2kstuwx" \
                   "222222111111444444333333555555666666"
) | ${0%/*}/validate
