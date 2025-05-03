CC = g++
FLAGS = -std=c++17 -Wall -O3
TARGET = HuffmanTool

all: 
	$(CC) $(FLAGS) -o obj/tree.o -c src/tree.cpp
	$(CC) $(FLAGS) -o obj/file.o -c src/file.cpp
	$(CC) $(FLAGS) -o obj/table.o -c src/table.cpp
	$(CC) $(FLAGS) -o obj/utils.o -c src/utils.cpp
	$(CC) $(FLAGS) -o obj/config.o -c src/config.cpp
	$(CC) $(FLAGS) -o obj/main.o -c src/main.cpp
	$(CC) $(FLAGS) -o bin/$(TARGET) obj/tree.o obj/file.o obj/table.o obj/utils.o obj/config.o obj/main.o
clean:
	rm -rf output/* obj/*.o bin/${TARGET}