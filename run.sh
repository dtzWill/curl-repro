#!/bin/sh

ITERS=20

# XXX: Test each variant
NIX_UNDER_TEST=$PWD/git-tests

BIN=$NIX_UNDER_TEST/bin/test

set -e

for x in `seq $ITERS`; do
  $BIN > run.$x.log 2>&1 \
    || (echo "run $x failed!"; cat run.$x.log; exit 1)
done

