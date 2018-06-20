# curl-repro
reproducing libcurl crash

Previously CI was expected to "fail" since this was intended to reproduce a crash issue.

Currently, with use of patch suggested by upstream, it appears to no longer crash.

[![Build Status](https://travis-ci.org/dtzWill/curl-repro.svg?branch=master)](https://travis-ci.org/dtzWill/curl-repro)


Example of previous failure and corresponding ASAN report log:
https://travis-ci.org/dtzWill/curl-repro/builds/394289319
