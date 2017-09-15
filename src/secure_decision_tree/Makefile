
CC = g++
LD = g++
AR = ar

CPPFLAGS=-Wall -std=c++11 -DNDEBUG -g2 -I.. -I/usr/local/include -I.
LIBS=../fhe.a -lntl -lgmp -ljansson -lm -L/usr/local/lib
TARGET=SecureDecisionTree
OBJS=common/util.o \
    common/timing.o \
    common/JSONParser.o \
    tree/Node.o \
    tree/Tree.o \
    crypto/Crypto.o \
    crypto/SecComparison.o


all: $(TARGET)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<


$(TARGET): main.cpp $(OBJS) ../fhe.a
	$(CC) $(CPPFLAGS) -o $@ $< $(OBJS) $(LIBS)


clean:
	rm -f $(TARGET) *.o *.exe *.a core.* tree/*.o common/*.o crypto/*.o
	rm -rf $(TARGET).dSYM
