SRC=src
CXX=g++
CFLAGS=-O -std=c++11 -Wall
LIB=-lcrypto -lncurses

ifeq ($(OS),Windows_NT)
	LIB+=-lWs2_32 -lgdi32 -lpdcurses
endif

all: enano

enano: $(SRC)/console.cpp $(SRC)/crypto.cpp $(SRC)/editor.cpp $(SRC)/file.cpp $(SRC)/main.cpp
	$(CXX) $(CFLAGS) $^ $(LIB) -o $@

clean:
	-rm -rf enano enano.exe
