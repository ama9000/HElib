#CFLAGS = -g -O2 -std=c++11 -Wfatal-errors -Wshadow -Wall -I/usr/local/include
HElibDir = ../
CPPFLAGS=-Wall -std=c++11 -DNDEBUG -g2 -I$(HElibDir) -I/usr/local/include -I.
LIBS=$(HElibDir)/fhe.a -lntl -lgmp -lm
TARGET=secComp

.PHONY: all clean

all: $(TARGET)

$(TARGET): main.cpp
	g++ $(CPPFLAGS) $< -o $(TARGET) $(LIBS) 

clean:
	rm -f $(TARGET)
