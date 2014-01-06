.PHONY: clean

CFLAGS = -I. -I.. -DDEBUG --stack-size 128 --stack-auto
CC = sdcc
VPATH = .:..

SOURCES = $(wildcard test_*.c)
TARGETS = $(subst .c, , $(SOURCES))

$(TARGETS):

test_% : test_%.c

clean:
	\rm -f *.[lamors]*
