CC = gcc
CFLAGS = -std=c99 -Wall -g -rdynamic -Werror 
LIBS = -lm -pthread

TARGET = compiler

.PHONY: default all clean

default:	$(TARGET)
all:		default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f valgrind-out.txt

debug:
	-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt $(TARGET)

clean:
	rm -f $(TARGET) *.o *.s out

test: $(TARGET) input01 input02
	./$(TARGET) input01
	cc -o out out.s
	./out
	./$(TARGET) input02
	cc -o out out.s
	./out

