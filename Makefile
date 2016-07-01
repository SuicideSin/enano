SRC=src
CXX=g++
CFLAGS=-O -std=c++11 -Wall
LIB=-lcrypto
INC=include
PD=

ifeq ($(OS),Windows_NT)
	PD+=$(INC)/pdcurses/win32/*.c
	CFLAGS+=-I $(INC)/pdcurses -I $(INC)/pdcurses/win32 -Wno-unused-but-set-variable
	LIB+=-lpdcurses -lgdi32
else
	LIB+=-lncurses
	ifneq ($(shell uname),Darwin)
		CFLAGS+=-Wno-unused-but-set-variable
	endif
endif

all: enano

enano: $(PD) $(SRC)/console.cpp $(SRC)/crypto.cpp $(SRC)/editor.cpp $(SRC)/file.cpp $(SRC)/main.cpp
	$(CXX) $(CFLAGS) $^ $(LIB) -o $@

clean:
	-rm -rf enano enano.exe
