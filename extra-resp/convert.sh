# !/bin/bash

echo -n "# 0.0 "
grep Interval $1 | sed -E "s/[^0-9,.]//g" | sed -E "s/,/\./"
tail $1 -n +10 | cut -f 2 | sed -E "s/,/\./"
