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
INCLUDES             = -I. $(INCS) 
TARGET               = main  
SOURCES		         = main.c
SRC = .
INC = .

.PHONY: all clean cleanall 

all:$(TARGET)

main: main.c
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o seq.o mandel.cpp $(LIBS)
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS)  -o seq seq.o $(LIBS)

pipeline:
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o pipeline.o mandel_pipeline.cpp $(LIBS) -lpthread
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o pipeline pipeline.o $(LIBS) -lpthread

farm:
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o farm.o mandel_farm.cpp $(LIBS) -lpthread
	$(CXX)  $(CXXFLAGS) $(OPTIMIZE_FLAGS)  $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o farm farm.o $(LIBS) -lpthread

clean:
	rm -f *.o

cleanall: clean
	rm -fr $(TARGET) *~ *.dSYM

depend:
	makedepend $(SOURCES)