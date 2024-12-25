CC=gcc
CFLAGS=-g -Wall -I./raylib//include -lGL -lm -lpthread -ldl -lrt -lX11
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/*.c, $(OBJ)/%.o, $(SRCS))
BIN=bin/main

all:$(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ ./raylib/lib/libraylib.a

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r bin/* obj/*
