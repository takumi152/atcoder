#!/usr/bin/env bash

testCount=999
score=0

echo "Starting system test..."
cd tools

for n in $(seq -f %04g 0 $testCount); do
  printf "Test ${n}/${testCount}: "
  cargo run --release --bin tester --quiet in/$n.txt ../$1 > out/$n.txt 2> /dev/null
  if [ $? -ne 0 ]; then
    printf "\e[93mRE\e[m %10d\n" 0
    continue
  fi

  cargo run --release --bin vis --quiet in/$n.txt out/$n.txt > out/temp_$n.txt 2> /dev/null
  testcaseScore=$(cat out/temp_$n.txt)
  score=$(python -c "print($score + $testcaseScore)")
  if [ $score -ne 0 ]; then
    printf "\e[92mAC\e[m %10d\n" $testcaseScore
  else
    printf "\e[93mWA\e[m %10d\n" 0
  fi
  rm out/temp_$n.txt

done

printf "Total score: %13d\n" $score
