#!/bin/bash

while :; do
  python arc104c_gen.py > input.txt
  res=$(./a < input.txt)
  echo $res
  if [[ $res != "Yes" ]]; then
    break
  fi
done
