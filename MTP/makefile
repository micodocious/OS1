# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#CFLAGS  = -std=c99 -Wall -Wextra -Wpedantic -Werror
CFLAGS  = -std=c99 -lpthread -lm

# the build target executable:
TARGET = line_processor

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
	$(RM) *.o

