SRC=src
CXX=g++
CFLAGS=-O -std=c++11 -Wall
LIB=-lcrypto

ifeq ($(OS),Windows_NT)
	LIB+=-lpdcurses -lgdi32 -Wno-unused-but-set-variable
else
	LIB+=-lncurses
	ifneq ($(shell uname),Darwin)
		LIB+=-Wno-unused-but-set-variable
	endif
endif

all: enano

enano: $(SRC)/console.cpp $(SRC)/crypto.cpp $(SRC)/editor.cpp $(SRC)/file.cpp $(SRC)/main.cpp
	$(CXX) $(CFLAGS) $^ $(LIB) -o $@

clean:
	-rm -rf enano enano.exe
