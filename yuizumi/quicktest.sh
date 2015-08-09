#!/bin/bash
(
    python json2cp.py
    echo 1
    echo "$1 $2"
    echo "$3" | tr "p'\!.03bcefy2aghij4lmno 5dqrvz2kstuwx" \
                   "222222111111444444333333555555666666"
) | ./validate
