CC = gcc -m32 -no-pie -fno-pie
NASM = nasm
CFLAGS = -g -std=gnu99 -O2
NASMFLAGS = -fwin32

CFLAGS_COMMON += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
	-Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
	-Wstack-usage=4096 -Wmissing-prototypes -Wfloat-equal -Wunused-value

CFLAGS_SANITIZE = -fsanitize=undefined -fsanitize-undefined-trap-on-error
CFLAGS = $(CFLAGS_COMMON) $(CFLAGS_SANITIZE)
CFLAGS_TEST = $(CFLAGS_COMMON) -DTEST_MODE

LDLIBS = -lm

.PHONY: all clean main test

all: main test clean

main: integral

test: integral_test
	./integral_test

integral: main.o compute.o func.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

integral_test: test_mode.o compute.o
	$(CC) $(CFLAGS_TEST) $^ -o $@ $(LDLIBS)

%.o: %.c functions.h
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLIBS)

test_mode.o: test_mode.c functions.h
	$(CC) $(CFLAGS_TEST) -c $< -o $@ $(LDLIBS)

func.o: func.asm
	$(NASM) $(NASMFLAGS) $< -o $@

clean:
	rm -f *.o
