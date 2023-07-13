# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#CFLAGS  = -std=c99 -Wall -Wextra -Wpedantic -Werror
CFLAGS  = -std=c99

# the build target executable:
TARGET0 = enc_server
TARGET1 = enc_client
TARGET2 = dec_server
TARGET3 = dec_client
TARGET4 = keygen

all: $(TARGET4)
all: $(TARGET0)
all: $(TARGET1)
all: $(TARGET2)
all: $(TARGET3)

$(TARGET4): $(TARGET4).c
	$(CC) $(CFLAGS) -o $(TARGET4) $(TARGET4).c

$(TARGET0): $(TARGET0).c
	$(CC) $(CFLAGS) -o $(TARGET0) $(TARGET0).c

$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c

$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c

$(TARGET3): $(TARGET3).c
	$(CC) $(CFLAGS) -o $(TARGET3) $(TARGET3).c

clean:
	$(RM) $(TARGET0)
	$(RM) $(TARGET1)
	$(RM) $(TARGET2)
	$(RM) $(TARGET3)
	$(RM) $(TARGET4)
	$(RM) *.o
