CC = g++
CCFLAGS = -std=c++11 -Wall
CCLIBR = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
TARGET = tetris.out
DEBUG_TARGET = debug_tetris.out

$(TARGET) : main.o game.o shape.o tests.o
	$(CC) main.o game.o shape.o tests.o -o $(TARGET) $(CCLIBR)
main.o : main.cpp game.hpp tests.hpp
	$(CC) $(CCFLAGS) -c main.cpp
game.o: game.cpp game.hpp shape.hpp
	$(CC) $(CCFLAGS) -c game.cpp
shape.o: shape.cpp shape.hpp
	$(CC) $(CCFLAGS) -c shape.cpp
tests.o: tests.cpp tests.hpp game.hpp shape.hpp
	$(CC) $(CCFLAGS) -c tests.cpp

.PHONY: run, clean
run: $(TARGET)
	./$(TARGET)
clean:
	rm -f *.o
	rm -f *.out