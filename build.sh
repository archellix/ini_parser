#!/bin/sh

OUTDIR=_out
DOCSDIR=docs
DEFAULT_TARGET=default
BASEDIR=$(dirname $0)

function main {
  if [ "$1" == "" ]; then
    TARGET=$DEFAULT_TARGET
  else
    TARGET=$1
  fi
  
  $TARGET
}

function build {
  mkdir -p $OUTDIR
  cd $OUTDIR
  cmake ..
  make
}

function tests {
  cd $OUTDIR
  ctest --output-on-failure
}

function docs {
  cd $BASEDIR
  doxygen doxyfile

}

function clean {
  rm -rf $OUTDIR
}

function default {
  build && tests
}

function publish {
  docs
  mkdir -p $DOCSDIR
  cp -R $OUTDIR/docs/html/* $DOCSDIR
}

cd $BASEDIR
main $*