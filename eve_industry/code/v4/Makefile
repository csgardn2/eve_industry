EXENAME=eve_industry
COMPILEFLAGS=-Wall -Wextra -ansi -Wfatal-errors -std=c++1z -g
LINKFLAGS=-Wall -Wextra -ansi -Wfatal-errors
LIBS=-lcurl
ARGS=--mode fetch-item-ids --item-ids-out data/json/item_ids.json

BINDIR=bin
DATADIR=data
DOCDIR=documentation
SOURCEDIR=source
COMPILER=g++-7

all: $(DOCDIR)/html_out/index.html $(BINDIR)/$(EXENAME)

# Add additional .o files on the line below (after main.o)
$(BINDIR)/$(EXENAME):           \
    $(BINDIR)/args.o            \
    $(BINDIR)/https_get.o       \
    $(BINDIR)/item_id.o         \
    $(BINDIR)/item_ids.o        \
    $(BINDIR)/json.o            \
    $(BINDIR)/main.o            \
    
	$(COMPILER) $(LINKFLAGS) -o $@ $^ $(LIBS)
	@echo "*** COMPILE_SUCCESSFUL ***"

#########################
# Main source tree

# Add more .cpp -> .o compile commands here

$(BINDIR)/args.o: $(SOURCEDIR)/args.cpp $(SOURCEDIR)/args.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/https_get.o: $(SOURCEDIR)/https_get.cpp $(SOURCEDIR)/https_get.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/item_id.o: $(SOURCEDIR)/item_id.cpp $(SOURCEDIR)/item_id.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/item_ids.o: $(SOURCEDIR)/item_ids.cpp $(SOURCEDIR)/item_ids.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/json.o: $(SOURCEDIR)/jsoncpp.cpp $(SOURCEDIR)/json.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/main.o: $(SOURCEDIR)/main.cpp
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

Doxyfile:
	@echo "Couldn't find Doxyfile, generating default"
	doxygen -g

#########################
# Convenience targets

clean:
	@rm -fv $(BINDIR)/*.o $(BINDIR)/$(EXENAME)

run:  $(DOCDIR)/html_out/index.html $(BINDIR)/$(EXENAME)
	$(BINDIR)/$(EXENAME) $(ARGS)

valgrind:  $(DOCDIR)/html_out/index.html $(BINDIR)/$(EXENAME)
	valgrind --suppressions=$(DATADIR)/valgrind_suppressions.supp --leak-check=full $(BINDIR)/$(EXENAME) $(ARGS)

$(DOCDIR)/html_out/index.html:      \
    $(DOCDIR)/mainpage.md           \
    $(DOCDIR)/Doxyfile              \
    $(SOURCEDIR)/args.h             \
    $(SOURCEDIR)/args.cpp           \
    $(SOURCEDIR)/https_get.h        \
    $(SOURCEDIR)/https_get.cpp      \
    $(SOURCEDIR)/item_id.h          \
    $(SOURCEDIR)/item_id.cpp        \
    $(SOURCEDIR)/item_ids.h         \
    $(SOURCEDIR)/item_ids.cpp       \
    $(SOURCEDIR)/json.h             \
    $(SOURCEDIR)/jsoncpp.cpp        \
    $(SOURCEDIR)/main.cpp           \
    
	doxygen $(DOCDIR)/Doxyfile

