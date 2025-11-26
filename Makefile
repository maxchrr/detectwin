CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -I.

SRC = compare_by_data.c compare_by_name.c core.c delete_file.c displayDir.c move_file.c
OBJ = $(SRC:.c=.so)
EXEC = detectwin

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) -o $@ $^

%.so: $(SRC)
	$(CC) $(CFLAGS) -fpic -shared -c $< -o $@

clean:
	@rm -f *.so $(EXEC)

.PHONY: all clean
