DIR_FONTS = ./Fonts
DIR_OBJ = ./screen

TARGET = watch
OBJ_C = $(wildcard ${DIR_FONTS}/*.c ${DIR_OBJ}/*.c)

CC = gcc

CFLAGS = -ggdb3

LIB = -lwiringPi -lm

${TARGET}:${OBJ_C}
	$(CC) $(CFLAGS) $^  -o $@ $(LIB)


	
clean :
	rm $(TARGET) 