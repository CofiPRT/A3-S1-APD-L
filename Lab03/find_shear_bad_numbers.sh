#!/bin/bash

indices=()

if [ $# -ne 1 ]; then
	echo "Incorrect number of args (expected: 1, found: $#)"
	exit 1
fi

make shear_bad &> /dev/null

iter=$1

for i in `seq 0 $1`
do
	echo -n "Current: $i.........."
	./shear_bad $i 2 | grep "incorecta" &> /dev/null

	if [ $? -eq 0 ]; then
		echo "INCORRECT ======================== "
		indices+=($i)
	else
		echo "done"
	fi

done

if [ ${#indices} -gt 0 ]; then
	echo "Incorrect indices: ${indices[@]}"
else
	echo "Incorrect indices: none"
fi

make clean &> /dev/null