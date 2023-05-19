CC=gcc
FLAGS= -Wall -ansi -pedantic --std=c90
.DEFAULT_GOAL := all

lib_target:
	@echo making lib
	$(MAKE) -C lib build

core_lib:
	@echo making core files
	$(MAKE) -C core build

assembler.o: lib_target core_lib
	@echo "compile main:"
	$(CC) -c assembler.c $(FLAGS)

all: clean assembler.o
	@echo "link object: assembler.o and all lib *.o:"
	$(CC) -o assembler assembler.o lib/*.o core/*.o

clean:
	rm -rf */**.o

run: all
	./assembler \
	./examples/mmnexample_1 \
	 ./examples/mmnexample \
	  ./examples/macro \
	   ./examples/comments \
	   ./examples/simple \
	   ./examples/one_command

test: clean all
	$(CC) -DUNIT_TEST unit_tests.c 
	$(CC) -o unit_tests assembler.o unit_tests.o
	./unit_tests