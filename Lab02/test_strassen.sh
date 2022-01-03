#!/bin/bash

N=1000
P=2

if [ ! -f "strassen" ]
then
    echo "Nu exista binarul strassen"
    exit
fi

if [ ! -f "strassen_par" ]
then
    echo "Nu exista binarul strassen_par"
    exit
fi

echo -n "Executing strassen..........."
./strassen $N > seq.txt
echo "DONE"

echo -n "Executing strassen_par......."
./strassen_par $N > par.txt
echo "DONE"

diff seq.txt par.txt

if [ $? -eq 0 ]; then
	echo "OK"
else
	echo "FAIL"
fi

rm -rf seq.txt par.txt