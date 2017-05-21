COMPILER=g++
EXENAME=eve_industry
COMPILEFLAGS=-fopenmp -Wall -Wextra -ansi -Wfatal-errors -std=c++11 -g
LINKFLAGS=-fopenmp -Wall -Wextra -ansi -Wfatal-errors -g
LIBS=-lcurl
ARGS=--region domain --station 60008494

all: documentation $(EXENAME)

# add additional .o files on the line below (after main.o)
$(EXENAME): \
    arguments.o \
    blueprint.o \
    calculate_blueprint_profitability.o \
    fetch_item_ids.o \
    fetch_orders.o \
    ftps_get.o \
    json.o \
    main.o \
    parse_price_list.o \
    print_blueprint_profitability.o \
    read_blueprints_file.o \
    token.o \
    utility.o
	$(COMPILER) $(LINKFLAGS) -o $(EXENAME) $^ $(LIBS)
	@echo "*** COMPILE_SUCCESSFUL ***"

#########################

# add more .cpp -> .o compile commands here

arguments.o: arguments.cpp arguments.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

blueprint.o: blueprint.cpp blueprint.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

calculate_blueprint_profitability.o: calculate_blueprint_profitability.cpp calculate_blueprint_profitability.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

fetch_item_ids.o: fetch_item_ids.cpp fetch_item_ids.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

fetch_orders.o: fetch_orders.cpp fetch_orders.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

ftps_get.o: ftps_get.cpp ftps_get.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

json.o: json.cpp json.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

main.o: main.cpp arguments.h json.h region.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

parse_price_list.o: parse_price_list.cpp parse_price_list.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

print_blueprint_profitability.o: print_blueprint_profitability.cpp print_blueprint_profitability.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

read_blueprints_file.o: read_blueprints_file.cpp read_blueprints_file.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

token.o: token.cpp token.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

utility.o: utility.cpp utility.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

Doxyfile:
	@echo "Couldn't find Doxyfile, generating default"
	doxygen -g

#########################

validate: testbenches/test_token
	valgrind --suppressions=valgrind_suppressions.supp --leak-check=full testbenches/test_token

testbenches/test_token: testbenches/test_token.cpp token.o
	$(COMPILER) $(COMPILEFLAGS) -o $@ -I./ $^

#########################

clean:
	@rm -fv *.o $(EXENAME)
	@rm -fv testbenches/test_token

run: $(EXENAME) documentation
	./$(EXENAME) $(ARGS)

# These suppressions occur whenever <iostream> is included.  Don't suppress
# anything else, it's probably a real problem with your code.
valgrind: $(EXENAME) documentation
	valgrind --suppressions=valgrind_suppressions.supp --leak-check=full ./$(EXENAME) $(ARGS)

documentation: Doxyfile $(ls *.cpp *.h)
	doxygen Doxyfile

