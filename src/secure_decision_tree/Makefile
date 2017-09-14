
CC = g++
LD = g++
AR = ar

CPPFLAGS=-Wall -std=c++11 -DNDEBUG -g2 -I.. -I/usr/local/include -I.
LIBS=../fhe.a -lntl -lgmp -lm -L/usr/local/lib
TARGET=SecureDecisionTree

all: $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

$(TARGET): main.cpp
	$(CC) $(CPPFLAGS) $< -o $(TARGET) $(LIBS)

clean:
	rm -rf $(TARGET) $(TARGET).dSYM
