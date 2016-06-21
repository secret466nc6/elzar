#!/bin/bash

BENCH=pca
NAME=$BENCH/$BENCH
ARGS=" -r 20 -c 20"
GOLDENRUN=$BENCH/goldenrun.log

source create_trace.sh

python fi-gdb.py -f -o 10000 -m nop  -p $NAME.native.exe -a "$ARGS" -d $NAME.native.log -r $GOLDENRUN -l logs/native &
wait
python fi-gdb.py -f -o 10000 -m nop  -p $NAME.avxswift.exe -a "$ARGS" -d $NAME.avxswift.log -r $GOLDENRUN -l logs/avxswift &
wait

