CC=gcc
all: mymount.c
	${CC} -Wall -fPIC -shared -o mymount.so mymount.c -ldl -D_FILE_OFFSET_BITS=64