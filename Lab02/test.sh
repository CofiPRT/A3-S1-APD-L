#!/bin/bash

N=1000
P=2

if [ ! -f "multiply_seq" ]
then
    echo "Nu exista binarul multiply_seq"
    exit
fi

if [ ! -f "multiply_i" ]
then
    echo "Nu exista binarul multiply_i"
    exit
fi

if [ ! -f "multiply_j" ]
then
    echo "Nu exista binarul multiply_j"
    exit
fi

if [ ! -f "multiply_k" ]
then
    echo "Nu exista binarul multiply_k"
    exit
fi

echo -n "Executing multiply_seq.........."
./multiply_seq $N > seq.txt
echo "DONE"

echo -n "Executing multiply_i............"
./multiply_i $N $P > par_i.txt
echo "DONE"

echo -n "Executing multiply_j............"
./multiply_j $N $P > par_j.txt
echo "DONE"

echo -n "Executing multiply_k............"
./multiply_k $N $P > par_k.txt
echo "DONE"

diff seq.txt par_i.txt

if [ $? -eq 0 ]; then
	echo "OK i"
else
	echo "FAIL"
fi

diff seq.txt par_j.txt

if [ $? -eq 0 ]; then
	echo "OK j"
else
	echo "FAIL"
fi

diff seq.txt par_k.txt

if [ $? -eq 0 ]; then
	echo "OK k"
else
	echo "FAIL"
fi

rm -rf seq.txt par_i.txt par_j.txt par_k.txt