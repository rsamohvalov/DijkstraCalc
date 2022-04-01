CC = g++
CFLAGS  = -g -Wall

SRC = DijkstraCalc.cpp list.cpp stack.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = DijkstraCalc

LDFLAGS = -lm

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJ) DijkstraCalc 