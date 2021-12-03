DIR := ${CURDIR} 

CC                   = gcc 
CXX                  = g++ 
LINK_OPT             = 
VERSION              = 
CXXFLAGS             = -std=c++1y 
CFLAGS               = $(DIS) 
LDFLAGS              = $(DIS)  
INCS                 = -I build/libraries/ 	
LIBS                 = -lm 
INCLUDES             = -I. $(INCS) DIR/gnulib/lib/
TARGET               = main  
SOURCES		         = main.c
SRC = .
INC = .

.PHONY: all clean cleanall 

all:$(TARGET)

main: main.c
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o main.o main.c $(LIBS)

clean:
	rm -f *.o

cleanall: clean
	rm -fr $(TARGET) *~ *.dSYM

depend:
	makedepend $(SOURCES)