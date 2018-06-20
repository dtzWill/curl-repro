#!/bin/sh

ITERS=10
BIN=$PWD/result/bin/test

set -e

for x in `seq $ITERS`; do
  $BIN > run.$x.log 2>&1 \
    || (echo "run $x failed!"; cat run.$x.log; exit 1)
done

