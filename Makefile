SRC=src
CXX=g++
CFLAGS=-O -std=c++11 -Wall -Wno-unused-but-set-variable
LIB=-lcrypto

ifeq ($(OS),Windows_NT)
	LIB+=-lpdcurses -lgdi32
else
	LIB+=-lncurses
endif

all: enano

enano: $(SRC)/console.cpp $(SRC)/editor.cpp $(SRC)/file.cpp $(SRC)/main.cpp
	$(CXX) $(CFLAGS) $^ $(LIB) -o $@

clean:
	-rm -rf enano enano.exe
