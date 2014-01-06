.PHONY: clean

CFLAGS = -I. -I.. -DDEBUG
CC = sdcc
VPATH = .:..

SOURCES = $(wildcard test_*.c)
TARGETS = $(subst .c, , $(SOURCES))

$(TARGETS):

test_% : test_%.c

clean:
	\rm -f *.[lamors]*
