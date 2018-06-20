#!/bin/sh

ITERS=10
BIN=$PWD/result/bin/test

set -e

for x in `seq $ITERS`; do
  $BIN >& run.$x.log
done

