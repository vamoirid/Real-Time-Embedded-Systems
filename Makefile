SHELL := /bin/bash

# ------------------------------------------------------------------------------
# COMMANDS
# ------------------------------------------------------------------------------
CC = gcc
RM = rm -f
CFLAGS = -Wall -Og
CLINKS = -lm -lpthread

# ------------------------------------------------------------------------------
# DIRECTORIES
# ------------------------------------------------------------------------------
SRC = src
BIN = bin

# ------------------------------------------------------------------------------
# TARGETS
# ------------------------------------------------------------------------------

all: prod-cons

# final link for executable
prod-cons: prod-cons.o
	$(CC) $(CFLAGS) $^ -o $(BIN)/$@ $(CLINKS)
# generate objects
%.o: $(SRC)/%.c
	$(CC) -c $< 

# clean temporary files
clean:
	$(RM) *.o *~

# remove executable
purge: clean
	$(RM) $(BIN)/prod-cons