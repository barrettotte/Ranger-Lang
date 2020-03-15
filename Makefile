CC = gcc
CFLAGS = -Wall -g -rdynamic -Werror 
LIBS = -lm -pthread

TARGET = rangerlang
SRCDIR = src
TESTDIR = tests

.PHONY: default all clean

default:	$(TARGET)
all:		default

OBJECTS = $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
HEADERS = $(wildcard $(SRCDIR)/*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@
	
clean:
	-rm -f $(SRCDIR)*.o
	-rm -f $(TESTDIR)/*.txt
	-rm -f $(TESTDIR)/*.asm
	-rm -f $(TARGET)
	-rm -f valgrind-out.txt
