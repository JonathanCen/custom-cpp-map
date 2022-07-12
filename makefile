TARGET = basic_test
HPP = Map

CC = g++
CFLAGS = -g -Wall -Wextra -pedantic -O

all: $(TARGET)

$(TARGET): $(TARGET).cpp $(HPP).hpp
	$(CC) $(CFLAGS) $(TARGET).cpp $(HPP).hpp -o $(TARGET)

clean:
	rm -rf  $(TARGET)