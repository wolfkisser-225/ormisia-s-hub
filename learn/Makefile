# 全类型排序系统 Makefile

CC = gcc
CFLAGS = -Wall -O2

OBJECTS = bubblesort.o md5.o test_data_generator.o
TARGET = bubblesort

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

bubblesort.o: bubblesort.c test_data.h md5.h
	$(CC) $(CFLAGS) -c $<

md5.o: md5.c md5.h
	$(CC) $(CFLAGS) -c $<

test_data_generator.o: test_data_generator.c test_data.h test_data_generator.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean