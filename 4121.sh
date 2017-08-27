#!/bin/bash

file=assets/times/range_4_20.csv
echo "Executing time" > ${file}

for i in 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20; do
    time=$(./4121)
    echo "$time" >> ${file}
done