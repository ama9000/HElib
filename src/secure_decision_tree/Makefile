
CC = g++
LD = g++
AR = ar

CPPFLAGS=-Wall -std=c++11 -DNDEBUG -g2 -I.. -I/usr/local/include -I.
LIBS=../fhe.a -lntl -lgmp -lm -L/usr/local/lib
TARGET=SecureDecisionTree
OBJS=util.o node.o tree.o


all: $(TARGET)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $<


$(TARGET): main.cpp $(OBJS) ../fhe.a
	$(CC) $(CPPFLAGS) -o $@ $< $(OBJS) $(LIBS)


clean:
	rm -f $(TARGET) *.o *.exe *.a core.*
	rm -rf $(TARGET).dSYM
