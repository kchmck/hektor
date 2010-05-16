OBJ = hektor-config.o         \
      hektor-error.o          \
      hektor-json-snapshots.o \
      hektor-modem.o          \
      hektor-path.o           \
      hektor-pep.o            \
      hektor-plan.o           \
      hektor-snapshot.o       \
      hektor-snapshots.o      \
      hektor-span.o           \
      hektor-usage.o          \
      hektor.o

EXECUTABLE = hektor
MANPAGE = hektor.1

LIBS = libcurl jansson libxdg-basedir lua

STD_CFLAGS  = -std=c99 -Wall -O2
STD_CFLAGS += $(shell pkg-config --cflags $(LIBS))

STD_LDFLAGS = $(shell pkg-config --libs $(LIBS))

ALL_CFLAGS  = $(STD_CFLAGS) $(CFLAGS)
ALL_LDFLAGS = $(STD_LDFLAGS) $(LDFLAGS)

PREFIX     = /usr
INSTALLDIR = $(DESTDIR)$(PREFIX)
BINDIR     = $(INSTALLDIR)/bin
MANDIR     = $(INSTALLDIR)/share/man/man1

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(ALL_LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(ALL_CFLAGS) -c $<

$(MANPAGE): $(MANPAGE).ronn
	ronn -b --manual="HEKTOR MANUAL" $<

manpage: $(MANPAGE)

install: all
	install -D $(EXECUTABLE) $(BINDIR)/$(EXECUTABLE)
	install -D $(MANPAGE) $(MANDIR)/$(MANPAGE)

uninstall:
	rm -f $(BINDIR)/$(EXECUTABLE)
	rm -f $(MANDIR)/$(MANPAGE)

clean:
	rm $(OBJ)

.PHONY: all manpage install uninstall clean
