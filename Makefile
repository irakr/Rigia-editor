# Makefile for the 'rigia-editor'

# Directories
SOURCE_DIR=./src/
BIN_DIR=./bin/
ALL_DIRS=$(SOURCE_DIR)

# Compiler and flags
CPP=g++
CFLAGS=-Wall -Wno-write-strings -O3 -std=c++11
FLAG_ALL_DIRS=-I$(SOURCE_DIR)
# Loader command
LD=ld

# Libraries
LIBC=-lc
LIBCPP=-lstdc++
LIB_NCURSES=-lncurses

# Object files
ALL_OBJS=$(SOURCE_DIR)editor.o $(SOURCE_DIR)io.o $(SOURCE_DIR)file_manip.o $(SOURCE_DIR)exceptions.o $(SOURCE_DIR)ncurses_wrapper.o

$(BIN_DIR)rig-editor: $(SOURCE_DIR)main.cc editor.o
	-mkdir $(BIN_DIR)
	$(CPP) $(CFLAGS) $(ALL_OBJS) $(SOURCE_DIR)main.cc -o $(BIN_DIR)rig-editor $(LIB_NCURSES)

editor.o: $(SOURCE_DIR)editor.cc $(SOURCE_DIR)io.o $(SOURCE_DIR)file_manip.o $(SOURCE_DIR)exceptions.o $(SOURCE_DIR)ncurses_wrapper.o
	$(CPP) $(CFLAGS) -c $(SOURCE_DIR)editor.cc $(SOURCE_DIR)io.o $(SOURCE_DIR)file_manip.o $(SOURCE_DIR)ncurses_wrapper.o \
	$(SOURCE_DIR)exceptions.o $(SOURCE_DIR)editor.cc $(LIB_NCURSES)
	
	mv editor.o $(SOURCE_DIR)

$(SOURCE_DIR)io.o: $(SOURCE_DIR)io.cc
	$(CPP) $(CFLAGS) -c $(SOURCE_DIR)io.cc -o $(SOURCE_DIR)io.o $(LIB_NCURSES)

$(SOURCE_DIR)file_manip.o: $(SOURCE_DIR)file_manip.cc
	$(CPP) $(CFLAGS) -c $(SOURCE_DIR)file_manip.cc -o $(SOURCE_DIR)file_manip.o

$(SOURCE_DIR)exceptions.o: $(SOURCE_DIR)exceptions.cc
	$(CPP) $(CFLAGS) -c $(SOURCE_DIR)exceptions.cc -o $(SOURCE_DIR)exceptions.o

$(SOURCE_DIR)ncurses_wrapper.o: $(SOURCE_DIR)ncurses_wrapper.cc
	$(CPP) $(CFLAGS) -c $(SOURCE_DIR)ncurses_wrapper.cc -o $(SOURCE_DIR)ncurses_wrapper.o $(LIB_NCURSES)

debug: $(SOURCE_DIR)editor.cc $(SOURCE_DIR)file_manip.cc $(SOURCE_DIR)io.cc $(SOURCE_DIR)exceptions.cc $(SOURCE_DIR)main.cc
	$(CPP) $(CFLAGS) -g $(SOURCE_DIR)editor.cc $(SOURCE_DIR)file_manip.cc \
	$(SOURCE_DIR)io.cc $(SOURCE_DIR)exceptions.cc $(SOURCE_DIR)main.cc -o test_module $(LIB_NCURSES)
	
clean:
	-rm *.o
	-rm $(SOURCE_DIR)*.o
	-rm -rf $(BIN_DIR)
	-rm test_module
