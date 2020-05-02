main: main.o scene.o object.o Monte_carlo.o
	g++ -std=c++17 -o main main.o scene.o object.o Monte_carlo.o

main.o: main.cpp scene.hpp
	g++ -std=c++17 -c main.cpp

scene.o: scene.cpp scene.hpp
	g++ -std=c++17 -c scene.cpp

object.o: object.cpp object.hpp
	g++ -std=c++17 -c object.cpp

Monte_carlo.o: Monte_carlo.cpp Monte_carlo.hpp
	g++ -std=c++17 -c Monte_carlo.cpp

clean:
	rm -f *.o *.exe

mrproper: clean
	rm -f main