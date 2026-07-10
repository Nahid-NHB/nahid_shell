CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g
SRCS = main.c helpers.c
OBJS = $(SRCS:.c=.o)
TARGET = nahid_shell

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c nahid_shell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
