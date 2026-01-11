CC = gcc
CFLAGS = -Wall -Wextra -O2

all: btime-to-xattr btime-from-xattr

btime-to-xattr: ./btime-to-xattr.c
	$(CC) $(CFLAGS) $^ -o $@

btime-from-xattr: ./btime-from-xattr.c
	$(CC) $(CFLAGS) $^ -o $@
