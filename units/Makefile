.PHONY:

CFLAGS = -g -I. -I.. -DUNIT_TEST -DDEBUG -fpack-struct -Wno-pointer-to-int-cast $(CMDLINE_CFLAGS)
CC = gcc
VPATH = .:..

SOURCES = $(wildcard test_*.c)
TARGETS = $(subst .c, , $(SOURCES))

$(TARGETS):

test_% : test_%.c %.c
