SHELL := /bin/bash

# ------------------------------------------------------------------------------
# COMMANDS
# ------------------------------------------------------------------------------
CC = gcc
RM = rm -f
CFLAGS = -Werror -Og
CLINKS = -lm -lpthread

# ------------------------------------------------------------------------------
# DIRECTORIES
# ------------------------------------------------------------------------------
SRC = src
BIN = bin

# ------------------------------------------------------------------------------
# TARGETS
# ------------------------------------------------------------------------------
EXECUTABLE = prod-cons

all: $(EXECUTABLE)

# final link for executable
prod-cons: prod-cons.o queue.o functionDataBase.o
	$(CC) $(CFLAGS) $^ -o $(BIN)/$@ $(CLINKS)
# generate objects
%.o: $(SRC)/%.c
	$(CC) -c $< 

# clean temporary files
clean:
	$(RM) *.o *~ 

# remove executable
purge: clean
	$(RM) $(BIN)/$(EXECUTABLE)