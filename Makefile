# This is a makefile. It is intended to better compile the program using one command: make
CC = g++

CFLAGS = -static-libgcc -static-libstdc++ -lwinmm

TARGET = commaxian

all: $(TARGET)

$(TARGET): $(TARGET).cpp
			$(CC) $(CFLAGS) -o $(TARGET).exe $(TARGET).cpp

clean:
			$(RM) $(TARGET)