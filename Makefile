CC = gcc
CFLAGS = -Wall -Wextra -O2

btime-to-xattr: ./btime-to-xattr.c
	$(CC) $(CFLAGS) $^ -o $@
