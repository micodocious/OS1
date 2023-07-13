CC     = gcc
CFLAGS = -std=c99 -ltree_testbench

SRCS = tree.c
OBJS = $(SRCS:.c=.o)
EXE  = tree

DBDIR     = debug
DBEXE     = $(DBDIR)/$(EXE)
DBOBJS    = $(addprefix $(DBDIR)/, $(OBJS))
DBCFLAGS  = -g -O0 -DDEBUG

REDIR     = build
REEXE     = $(REDIR)/$(EXE)
REOBJS    = $(addprefix $(REDIR)/, $(OBJS))
RECFLAGS  = -O3 -Wall -Wextra -Wpedantic -Werror

.PHONY: all clean debug release prep

all: debug release tree

tree: $(REEXE)
	@cp $< $@

debug: prep $(DBEXE)

$(DBEXE): $(DBOBJS)
	$(CC) $(CFLAGS) $(DBCFLAGS) -DTEST -o $@ $^

$(DBDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(DBCFLAGS) -o $@ $<


release: prep $(REEXE)

$(REEXE): $(REOBJS)
	$(CC) $(CFLAGS) $(RECFLAGS) -o $@ $^

$(REDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $(RECFLAGS) -o $@ $<

prep:
	@mkdir -p $(DBDIR) $(REDIR)

clean:
	rm -rf tree $(DBDIR) $(REDIR)
