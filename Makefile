BINARY = hektor

OBJS = \
  bstrlib.o \
  hektor.o \
  info.o \
  modem.o \

LIBS = libcurl

ALL_CFLAGS  = -pipe -std=c99 -O2 -Wall -Wextra -pedantic -Wno-switch \
              -Wno-pointer-sign
ALL_CFLAGS += $(shell pkg-config --cflags $(LIBS))
ALL_CFLAGS += $(CFLAGS)

ALL_LDFLAGS  = $(shell pkg-config --libs $(LIBS))
ALL_LDFLAGS += -lyajl
ALL_LDFLAGS += $(LDFLAGS)

PREFIX     = /usr
INSTALLDIR = $(DESTDIR)$(PREFIX)
BINDIR     = $(INSTALLDIR)/bin

$(BINARY): $(OBJS)
	$(CC) $(ALL_LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(ALL_CFLAGS) -o $@ -c $<

install: $(BINARY)
	install -D $(BINARY) $(BINDIR)/$(BINARY)

clean:
	-rm $(OBJS)

distclean: clean
	-rm $(BINARY)

.PHONY: install clean distclean
