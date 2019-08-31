CC = g++
CCFLAGS = -std=c++11 -Wall
CCLIBR = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
TARGET = tetris.out
DEBUG_TARGET = debug_tetris.out

$(TARGET) : main.o
	$(CC) main.o -o $(TARGET) $(CCLIBR)
main.o : main.cpp game.hpp
	$(CC) $(CCFLAGS) -c main.cpp

.PHONY: run, clean
run: $(TARGET)
	./$(TARGET)
clean:
	rm -f *.o
	rm -f *.out