CC = gcc
CC_FLAGS = -w -std=c11
EXEC = blackjack
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS) *~
