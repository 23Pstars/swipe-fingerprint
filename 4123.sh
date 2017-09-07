#!/bin/bash

range=$1
file=assets/times3/impostor/range_${range}.csv

echo "Executing time" > ${file}

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20; do
    times=$(./4123 ${range} ${i})
    echo "$i,$times" >> ${file}
done