# Contact conor.gardner@arm.com if you have questions about this code.

COMPILER=g++-7
EXENAME=blueprint_converter
COMPILEFLAGS=-Wall -Wextra -ansi -Wfatal-errors -std=c++1z -I source -g
LINKFLAGS=-Wall -Wextra -ansi -Wfatal-errors -g
LIBS=
ARGS=--ccp-yaml-in data/blueprints.yaml --custom-json-out data/blueprints.json

BINDIR=bin
DATADIR=data
DOCDIR=documentation
SOURCEDIR=source
LIBDIR=lib

#########################

all: $(DOCDIR)/html_out/index.html $(BINDIR)/$(EXENAME)
	@echo "*** COMPILE_SUCCESSFUL ***"

# Add additional .o files on the line below (after main.o)
#
# Libraries should be last, since g++ will ignore them unless they are first
# referenced by another source
$(BINDIR)/$(EXENAME):               \
    $(BINDIR)/args.o                \
    $(BINDIR)/main.o                \
    $(LIBDIR)/json.o                \
    $(LIBDIR)/libyaml-cpp.a         \
    
	$(COMPILER) $(LINKFLAGS) -o $(BINDIR)/$(EXENAME) $^ $(LIBS)

#########################

# add more .cpp -> .o compile commands here

$(BINDIR)/args.o: $(SOURCEDIR)/args.cpp $(SOURCEDIR)/args.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/main.o: $(SOURCEDIR)/main.cpp
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(DOCDIR)/Doxyfile:
	@echo "Couldn't find Doxyfile, generating default"
	doxygen -g $(DOCDIR)/Doxyfile

#########################

clean:
	@rm -fv $(BINDIR)/*.o $(BINDIR)/$(EXENAME)
	@rm -fr $(DOCDIR)/html_out

run: $(BINDIR)/$(EXENAME) $(DOCDIR)/html_out/index.html
	@echo "*** COMPILE_SUCCESSFUL ***"
	./$(BINDIR)/$(EXENAME) $(ARGS)

# These suppressions occur whenever <iostream> is included.  Don't suppress
# anything else, it's probably a real problem with your code.
valgrind: $(BINDIR)/$(EXENAME) $(DOCDIR)/html_out/index.html
	valgrind --suppressions=$(DATADIR)/valgrind_suppressions.supp --leak-check=full ./$(BINDIR)/$(EXENAME) $(ARGS)

$(DOCDIR)/html_out/index.html:  \
    $(DOCDIR)/Doxyfile          \
    $(SOURCEDIR)/args.cpp       \
    $(SOURCEDIR)/args.h         \
    $(SOURCEDIR)/main.cpp       \
    
	doxygen $(DOCDIR)/Doxyfile

