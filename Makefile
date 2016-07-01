SRC=src
CXX=c++
CFLAGS=-O -std=c++11 -Wall
LIB=-lncurses -lcrypto
INC=include

ifneq ($(shell uname),Darwin)
	CFLAGS+=-Wno-unused-but-set-variable
endif

all: enano

enano: $(SRC)/console.cpp $(SRC)/crypto.cpp $(SRC)/editor.cpp $(SRC)/file.cpp $(SRC)/main.cpp
	$(CXX) $(CFLAGS) $^ $(LIB) -o $@

clean:
	-rm -rf enano enano.exe
