COMPILER=g++
EXENAME=eve_industry
COMPILEFLAGS=-Wall -Wextra -ansi -Wfatal-errors -std=c++11
LINKFLAGS=-Wall -Wextra -ansi -Wfatal-errors
LIBS=-lcurl
ARGS=

SOURCEDIR=source
BINDIR=bin
DATADIR=data

all: documentation $(BINDIR)/$(EXENAME)

# Add additional .o files on the line below (after main.o)
$(BINDIR)/$(EXENAME): $(BINDIR)/https_get.o $(BINDIR)/main.o
	$(COMPILER) $(LINKFLAGS) -o $@ $^ $(LIBS)
	@echo "*** COMPILE_SUCCESSFUL ***"

#########################

# Add more .cpp -> .o compile commands here

$(BINDIR)/https_get.o: $(SOURCEDIR)/https_get.cpp $(SOURCEDIR)/https_get.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/main.o: $(SOURCEDIR)/main.cpp
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

Doxyfile:
	@echo "Couldn't find Doxyfile, generating default"
	doxygen -g

#########################

clean:
	@rm -fv $(BINDIR)/*.o $(BINDIR)/$(EXENAME)

run: $(BINDIR)/$(EXENAME)
	$(BINDIR)/$(EXENAME) $(ARGS)

valgrind: $(BINDIR)/$(EXENAME)
	valgrind --leak-check=full $(BINDIR)/$(EXENAME) $(ARGS)

documentation: documentation/Doxyfile $(ls *.cpp *.h) documentation/html_out/index.html

documentation/html_out/index.html:
	doxygen documentation/Doxyfile

