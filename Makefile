DIR := ${CURDIR} 

CC                   = gcc 
LINK_OPT             = 
VERSION              = 
CFLAGS               = $(DIS) 
LDFLAGS              = $(DIS)  
INCS                 = -I build/libraries/ 	
LIBS                 = -lm 
INCLUDES             = -I. $(INCS) 
TARGET               = tcg  
SOURCE		         = tcg.c
SRC = .
INC = .

.PHONY: clean cleanall 

tcg: $(SOURCE)
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o tcg tcg.c $(LIBS)

clean:
	rm -f $(TARGET)
