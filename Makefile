.POSIX:

include config.mk

# search src/ and libs/ for c files
SRC = $(shell find src libs -name '*.c' | grep -v src/main.c)
OBJ = ${SRC:.c=.o}

# tests live in t/
TESTS = $(shell find t -name '*.c' )
# output location for tests
TESTOUT = bin
TESTO = $(patsubst %.c, $(TESTOUT)/%, $(TESTS))

# only defined when `make debug`
EXTRAFLAGS =

# default to compiling without debug symbols
all: icarus

# compile each component, linking is handled by `icarus` rule
%.o: %.c
	@echo COMPILING CC $< with extra flags \"${EXTRAFLAGS}\"
	@${CC} -g -c ${CFLAGS} $< ${EXTRAFLAGS} -o $@

# compile and link main executable
icarus: ${OBJ}
	@echo more compiling CC -o $@ with extra flags \"${EXTRAFLAGS}\"
	@${CC} src/main.c -o $@ ${LDFLAGS} ${EXTRAFLAGS} ${OBJ}
	@make -s cleanobj

# build icarus with debug output
debug:
	@make -s EXTRAFLAGS="${DEBUGFLAGS}" icarus

# only clean up temporary files (.o)
cleanobj:
	@echo cleaning objects and temporary files
	@find . -iname '*.o' -delete

# clean up everything managed by make
clean: cleanobj
	@echo cleaning executables
	@rm -f icarus
	@echo cleaning tests
	@rm -rf $(TESTOUT)
	@echo removing gcov files
	@find . -iname '*.gcda' -delete
	@find . -iname '*.gcov' -delete
	@find . -iname '*.gcno' -delete

# run icarus over our simple example
example: icarus
	./icarus example/simple.ic

# clean
# build all OBJ
# run all TESTO
# cleanobj
#   this ensures that we do a fresh build every time
test: clean $(OBJ) $(TESTO) test_success cleanobj

# compile and run each test
$(TESTO) : $(TESTOUT)/% : %.c
	@echo "\n\ncompiling $< to $@"
	@mkdir -p `dirname $@`
	@${CC} $< -o $@ ${LDFLAGS} ${OBJ}
	@echo running test
	$@

# signal test success
test_success:
	@echo "\n\ntesting success\n"

.PHONY: all clean cleanobj icarus test example

