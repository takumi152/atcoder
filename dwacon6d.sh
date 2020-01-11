#!/usr/bin/env bash
# solution comparison

while :; do
  python dwacon6d_gen.py > input.txt
  ./a < input.txt > output.txt
  python dwacon6d.py < input.txt > output_stupid.txt
  diff -Z output.txt output_stupid.txt > diff.txt
  if [ -s diff.txt ]; then
    cat diff.txt
    break
  fi
done
