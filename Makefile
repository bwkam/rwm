.POSIX:
ALL_WARNING = -Wall -Wextra -pedantic
ALL_LDFLAGS = -lxcb -lxcb-keysyms $(LDFLAGS)
ALL_CFLAGS = $(CPPFLAGS) $(CFLAGS) -std=c99 $(ALL_WARNING)
PREFIX = /usr/local
LDLIBS = -lm
BINDIR = $(PREFIX)/bin
NAME = rwm

all: $(NAME)
install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	cp -f $(NAME) $(DESTDIR)$(BINDIR)
	chmod 755 $(DESTDIR)$(BINDIR)/$(NAME)
rwm: rwm.o
	$(CC) $(ALL_LDFLAGS) -o rwm rwm.o $(LDLIBS)
rwm.o: rwm.c rwm.h config.h
clean:
	rm -f rwm *.o
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/rwm
.PHONY: all install uninstall clean
