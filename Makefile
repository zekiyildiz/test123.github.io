# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = 

SRCS = main.c game.c player.c room.c item.c creature.c
OBJS = $(SRCS:.c=.o)

TARGET = game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

rebuild: clean all

.PHONY: all clean run rebuild