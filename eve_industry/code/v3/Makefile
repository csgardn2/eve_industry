COMPILER=g++
EXENAME=eve_industry
COMPILEFLAGS=-Wall -Wextra -ansi -Wfatal-errors -std=c++11
LINKFLAGS=-Wall -Wextra -ansi -Wfatal-errors
LIBS=
ARGS=

all: documentation $(EXENAME)

# Add additional .o files on the line below (after main.o)
$(EXENAME): json.o main.o token.o
	$(COMPILER) $(LINKFLAGS) -o $(EXENAME) $^ $(LIBS)
	@echo "*** COMPILE_SUCCESSFUL ***"

#########################

# Add more .cpp -> .o compile commands here

args.o: args.cpp args.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

json.o: json.cpp json.h
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

main.o: main.cpp
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

token.o: token.cpp
	$(COMPILER) $(COMPILEFLAGS) -c -o $@ $<

Doxyfile:
	@echo "Couldn't find Doxyfile, generating default"
	doxygen -g

#########################

clean:
	@rm -fv *.o $(EXENAME)

run: $(EXENAME)
	./$(EXENAME) $(ARGS)

valgrind: $(EXENAME)
	valgrind --leak-check=full ./$(EXENAME) $(ARGS)

documentation: Doxyfile $(ls *.cpp *.h)
	doxygen Doxyfile

