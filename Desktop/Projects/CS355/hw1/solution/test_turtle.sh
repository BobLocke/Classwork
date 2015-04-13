#!/bin/bash
FILES=tests/*.turtle

for f in $FILES
    do
        echo "interpreting $f program "
        ./interpreter < $f > outputs/${f##*/}.pgm
    done

