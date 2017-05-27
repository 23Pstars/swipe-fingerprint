#!/bin/bash

jump=$1
file=assets/results/2/${jump}/f_score.csv

echo "Genuine,Impostor" > ${file}

for i in 0 1 2 3 4 5; do
    for j in 0 1 2 3 4 5; do
        for k in 0 1 2 3 4 5; do
            for l in 0 1 2 3 4 5; do

                score=$(./match ${jump} ${i} ${j} ${k} ${l})

                if [ ${i} == ${k} ]; then
                    echo "$score,0" >> ${file}
                else
                    echo "0,$score" >> ${file}
                fi
            done
        done
    done
done