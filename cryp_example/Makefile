CPP    = g++
LIBDIR = 
CFLAGS = -Wall
LFLAGS = -O3 
LIBS   = -lgcrypt -lgpg-error

default: libgcrypt_example

.PHONY :    clean

files   := $(wildcard *.cpp)
objects := $(patsubst %.cpp,%.o,$(files))
Deps    := $(patsubst %.cpp,%.Po,$(files))

deps: $(Deps)
	@echo $@

-include $(Deps)

$(Deps): %.Po: %.cpp
	$(CPP) $(CFLAGS) -MM -MF $*.Po $<

$(objects): %.o: %.cpp
	$(CPP) -c $(CFLAGS) $< -o $@

libgcrypt_example : $(objects)
	$(CPP) $(LFLAGS) -o $@ $(objects) $(LIBS) 
	@echo
	@echo Build complete.
	@echo

clean:
	-rm -f *.o *.Po
	-rm -f libgcrypt_example libgcrypt_example.exe
