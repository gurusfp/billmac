.PHONY:

CFLAGS = -I. -I.. -DUNIT_TEST
CC = gcc
VPATH = .:..

SOURCES = $(wildcard test_*.c)
TARGETS = $(subst .c, , $(SOURCES))

$(TARGETS):

test_% : test_%.c %.c
