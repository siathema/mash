CC=clang++
CFLAGS=-std=c++11 -g -c
LFLAGS=-g -std=c++11
SOURCES := \
	src/mash.cc


LIBS=-lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE_PATH=builds/
EXECUTABLE=mash

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : 
	$(CC) $(LFLAGS) $(SOURCES) -o $(EXECUTABLE_PATH)$@ $(LIBS)

#$(SOURCES):
#	$(CC) $(CFLAGS) $@ -o $<

clean:
	rm src/*.o
	rm builds/debug/$(EXECUTABLE)
