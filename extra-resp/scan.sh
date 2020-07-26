# !/bin/bash

FILENAME=_Ada_1A_007Sel_U.txt

WINDOW=120
OFFSET=0
STOP=1020
HOP=60

while [ $OFFSET -le $STOP ]
do
    ./exe-ostd $FILENAME $OFFSET $WINDOW 2> /dev/null | grep "Final"
    let OFFSET=OFFSET+HOP
done
