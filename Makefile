.PHONY: clean

#
# Settings
#

NAME = pngdemo
TARGET_DIR = ./target
CC = gcc
LIBPNG_CFLAGS = $(shell libpng-config --cflags --ldflags)
CFLAGS = $(LIBPNG_CFLAGS)

#
# Static targets
#

all: main.cpp
	mkdir -p "$(TARGET_DIR)"
	$(CC) $(CFLAGS) -o "$(TARGET_DIR)/$(NAME)" main.cpp

clean:
	rm -rf "$(TARGET_DIR)"
