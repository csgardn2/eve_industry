EXENAME=eve_industry
COMPILEFLAGS=-Wall -Wextra -ansi -Wfatal-errors -std=c++1z -g -fopenmp
LINKFLAGS=-Wall -Wextra -ansi -Wfatal-errors
LIBS=-lcurl -fopenmp
ARGS=--mode fetch-item-attributes --item-attributes-out data/json/item_attributes.json

BINDIR=bin
DATADIR=data
DOCDIR=documentation
SOURCEDIR=source
COMPILER=g++-7

all: $(DOCDIR)/html_out/index.html $(BINDIR)/$(EXENAME)

# Add additional .o files on the line below (after main.o)
$(BINDIR)/$(EXENAME):                   \
    $(BINDIR)/args.o                    \
    $(BINDIR)/error.o                   \
    $(BINDIR)/https_get.o               \
    $(BINDIR)/item_attribute.o          \
    $(BINDIR)/item_attributes.o         \
    $(BINDIR)/item_ids.o                \
    $(BINDIR)/json.o                    \
    $(BINDIR)/main.o                    \
    $(BINDIR)/order.o                   \
    $(BINDIR)/regional_market_orders.o  \
    $(BINDIR)/station_attribute.o       \
    $(BINDIR)/station_attributes.o      \
    $(BINDIR)/util.o                    \
    
	$(COMPILER) $(LINKFLAGS) -o $@ $^ $(LIBS)
	@echo "*** COMPILE_SUCCESSFUL ***"

#########################
# Main source tree

# Add more .cpp -> .o compile commands here

$(BINDIR)/args.o: $(SOURCEDIR)/args.cpp $(SOURCEDIR)/args.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/error.o: $(SOURCEDIR)/error.cpp $(SOURCEDIR)/error.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/https_get.o: $(SOURCEDIR)/https_get.cpp $(SOURCEDIR)/https_get.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/item_attribute.o: $(SOURCEDIR)/item_attribute.cpp $(SOURCEDIR)/item_attribute.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/item_attributes.o: $(SOURCEDIR)/item_attributes.cpp $(SOURCEDIR)/item_attributes.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/item_ids.o: $(SOURCEDIR)/item_ids.cpp $(SOURCEDIR)/item_ids.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/json.o: $(SOURCEDIR)/jsoncpp.cpp $(SOURCEDIR)/json.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/main.o: $(SOURCEDIR)/main.cpp
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/order.o: $(SOURCEDIR)/order.cpp $(SOURCEDIR)/order.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/regional_market_orders.o: $(SOURCEDIR)/regional_market_orders.cpp $(SOURCEDIR)/regional_market_orders.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/station_attribute.o: $(SOURCEDIR)/station_attribute.cpp $(SOURCEDIR)/station_attribute.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/station_attributes.o: $(SOURCEDIR)/station_attributes.cpp $(SOURCEDIR)/station_attributes.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

$(BINDIR)/util.o: $(SOURCEDIR)/util.cpp $(SOURCEDIR)/util.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

Doxyfile:
	@echo "Couldn't find Doxyfile, generating default"
	doxygen -g

#########################
# Convenience targets

fetch_item_attributes: $(DOCDIR)/html_out/index.html $(BINDIR)/$(EXENAME)
	$(BINDIR)/$(EXENAME)                                            \
        --mode fetch-item-attributes                                \
        --item-attributes-out data/json/item_attributes.json

fetch_prices: $(DOCDIR)/html_out/index.html $(BINDIR)/$(EXENAME)
	$(BINDIR)/$(EXENAME)                                            \
        --mode fetch-prices                                         \
        --item-attributes-in data/json/item_attributes.json         \
        --station-attributes-in data/json/station_attributes.json   \
        --prices-out data/json/prices.json

clean:
	@rm -fv $(BINDIR)/*.o $(BINDIR)/$(EXENAME)

run:  $(DOCDIR)/html_out/index.html $(BINDIR)/$(EXENAME)
	$(BINDIR)/$(EXENAME) $(ARGS)

valgrind:  $(DOCDIR)/html_out/index.html $(BINDIR)/$(EXENAME)
	valgrind --suppressions=$(DATADIR)/valgrind_suppressions.supp --leak-check=full $(BINDIR)/$(EXENAME) $(ARGS)

$(DOCDIR)/html_out/index.html:              \
    $(DOCDIR)/mainpage.md                   \
    $(DOCDIR)/Doxyfile                      \
    $(SOURCEDIR)/args.h                     \
    $(SOURCEDIR)/args.cpp                   \
    $(SOURCEDIR)/error.h                    \
    $(SOURCEDIR)/error.cpp                  \
    $(SOURCEDIR)/https_get.h                \
    $(SOURCEDIR)/https_get.cpp              \
    $(SOURCEDIR)/item_ids.h                 \
    $(SOURCEDIR)/item_ids.cpp               \
    $(SOURCEDIR)/json.h                     \
    $(SOURCEDIR)/jsoncpp.cpp                \
    $(SOURCEDIR)/main.cpp                   \
    $(SOURCEDIR)/order.h                    \
    $(SOURCEDIR)/order.cpp                  \
    $(SOURCEDIR)/regional_market_orders.h   \
    $(SOURCEDIR)/regional_market_orders.cpp \
    $(SOURCEDIR)/station_attribute.h        \
    $(SOURCEDIR)/station_attribute.cpp      \
    $(SOURCEDIR)/station_attributes.h       \
    $(SOURCEDIR)/station_attributes.cpp     \
    $(SOURCEDIR)/util.h                     \
    $(SOURCEDIR)/util.cpp                   \
    
	doxygen $(DOCDIR)/Doxyfile

