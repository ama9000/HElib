
#CFLAGS = -g -O2 -std=c++11 -Wfatal-errors -Wshadow -Wall -I/usr/local/include
HEdir = /Users/asma/Desktop/HElib/src
#HElib = $(HEdir)/fhe.a
CPPFLAGS=-Wall -std=c++11 -DNDEBUG -g2 -I$(HEdir)
LIBS=$(HEdir)/fhe.a -lntl -lgmp -lm
TARGET=simpleSum

.PHONY: all clean

all: $(TARGET)

$(TARGET): main.cpp
	g++ $(CPPFLAGS) $< -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
