main: main.cpp
	g++ -std=c++17 main.cpp -o main -fopenmp -O3

clean:
	rm main

mrproper: clean
	rm -f main