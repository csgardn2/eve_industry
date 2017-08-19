COMPILER=g++
EXENAME=eve_industry
VALNAME=validator
COMPILEFLAGS=-Wall -Wextra -ansi -Wfatal-errors -std=c++11
LINKFLAGS=-Wall -Wextra -ansi -Wfatal-errors
LIBS=-lcurl
ARGS=

BINDIR=bin
DATADIR=data
DOCDIR=documentation
SOURCEDIR=source
VALDIR=validator

all: $(DOCDIR)/html_out/index.html $(BINDIR)/$(EXENAME) $(BINDIR)/$(VALNAME)

# Add additional .o files on the line below (after main.o)
$(BINDIR)/$(EXENAME):           \
    $(BINDIR)/args.o            \
    $(BINDIR)/https_get.o       \
    $(BINDIR)/main.o            \
    
	$(COMPILER) $(LINKFLAGS) -o $@ $^ $(LIBS)
	@echo "*** COMPILE_SUCCESSFUL ***"

$(BINDIR)/$(VALNAME):           \
    $(BINDIR)/args.o            \
    $(BINDIR)/https_get.o       \
    $(BINDIR)/validator.o       \
    $(BINDIR)/validate_args.o   \
    
	$(COMPILER) $(LINKFLAGS) -o $@ $^ $(LIBS)

#########################
# Main source tree

# Add more .cpp -> .o compile commands here

$(BINDIR)/args.o: $(SOURCEDIR)/args.cpp $(SOURCEDIR)/args.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/https_get.o: $(SOURCEDIR)/https_get.cpp $(SOURCEDIR)/https_get.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/main.o: $(SOURCEDIR)/main.cpp
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

Doxyfile:
	@echo "Couldn't find Doxyfile, generating default"
	doxygen -g

#########################
# Validator source tree

$(BINDIR)/validator.o: $(VALDIR)/validator.cpp $(VALDIR)/validator.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/validate_args.o: $(VALDIR)/validate_args.cpp $(SOURCEDIR)/args.cpp $(SOURCEDIR)/args.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

#########################
# Convenience targets

clean:
	@rm -fv $(BINDIR)/*.o $(BINDIR)/$(EXENAME)

run: $(BINDIR)/$(EXENAME)
	$(BINDIR)/$(EXENAME) $(ARGS)

valgrind: $(BINDIR)/$(EXENAME)
	valgrind --suppressions=$(DATADIR)/valgrind_suppressions.supp --leak-check=full $(BINDIR)/$(EXENAME) $(ARGS)

validate: $(BINDIR)/$(VALNAME)
	valgrind --suppressions=$(DATADIR)/valgrind_suppressions.supp --leak-check=full $(BINDIR)/$(VALNAME)

$(DOCDIR)/html_out/index.html:  \
    $(DOCDIR)/mainpage.md       \
    $(DOCDIR)/Doxyfile          \
    $(BINDIR)/$(VALNAME)        \
    $(BINDIR)/$(EXENAME)
    
	doxygen $(DOCDIR)/Doxyfile

