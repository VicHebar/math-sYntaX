CC = gcc
CFLAGS = -Wall -Wextra -g -fPIE
LDFLAGS = -pie

TARGET = math-sYntX.exe
SRCS = exp.c parser.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

math-sYntX.o: exp.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q /F $(OBJS) $(TARGET) 2>nul