default: build

build:
	mpic++ main.cpp -o main

run: build
	mpirun --oversubscribe -np 12 main

clean:
	rm main