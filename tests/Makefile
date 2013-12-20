.PHONY:

CFLAGS = -g -I. -I.. -DDEBUG
CC = gcc
VPATH = .:..

SOURCES = $(wildcard test_*.c)
TARGETS = $(subst .c, , $(SOURCES))

$(TARGETS):

test_% : test_%.c
