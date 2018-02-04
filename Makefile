
CC = clang
CC_FLAGS = -fshort-enums -std=c11 -O2 -Iinclude
#CC_FLAGS = -Weverything -Wno-padded -fshort-enums -std=c11 -O0 -g -Iinclude

#CC = gcc
#CC_FLAGS = -Wall -Wextra -fshort-enums -std=c11 -O0 -g -Iinclude

EXEC = blackjack
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS) *~
