#!/bin/bash

# I was too stupid to figure out the makefile black magic to do this...

make clean
make

for f in tests/*.rl
do
  if [[ $? -ne 0 ]] ; then
    exit 1
  fi
  ./rangerlang $f
done
