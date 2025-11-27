CC = cc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -I.

SRC = explorer.c compares.c items.c selection.c ui.c
OBJ = $(SRC:.c=.so)
EXEC = detectwin

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) -o $@ $^ -lncurses

%.so: $(SRC)
	$(CC) $(CFLAGS) -fpic -shared -c $< -o $@

clean:
	@rm -f *.so $(EXEC)

.PHONY: all clean
