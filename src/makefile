# set the C compiler, for C++ use g++
# use this setting on most sane machines
CC = gcc

# use this line on the engin linux machines
# CC = gcc296

# set the path to the include directory
INCDIR =../include

# set the flags for the C and C++ compiler to give lots of warnings
CFLAGS = -I$(INCDIR) -I/opt/local/include -O2 -Wall -Wstrict-prototypes -Wnested-externs -Wmissing-prototypes -Wmissing-declarations -g
CPPFLAGS = $(CFLAGS)

# path to the object file directory
ODIR = obj

# path to the lib directory
LIBDIR =../lib

# path to the bin directory
BINDIR =../bin

# libraries to include
LIBS = -lm -lgraphics
LFLAGS = -L$(LIBDIR) -L/opt/local/lib

# put all of the relevant include files here
_DEPS = ppmIO.h image.h graphics.h polygon.h transform.h viewing.h hierarchical_modeling.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of the executables here
EXECUTABLES = test6a test6b cube gif spaceship creative

# put a list of all the object files here for all executables (with .o endings)
_OBJ = test6a.o test6b.o cube.o gif.o spaceship.o creative.o

# convert them to point to the right place
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# patterns for compiling source code
# $< is the file that caused the action to occur
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


all:
	for i in $(EXECUTABLES); do (make $$i) ; done


# put the specific executable rules here
test6a: $(ODIR)/test6a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
test6b: $(ODIR)/test6b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
cube: $(ODIR)/cube.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
gif: $(ODIR)/gif.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
spaceship: $(ODIR)/spaceship.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
creative: $(ODIR)/creative.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
