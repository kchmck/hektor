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

CFLAGS += -std=c99 -Wall -O2
CFLAGS += $(shell pkg-config --cflags libcurl)
CFLAGS += $(shell pkg-config --cflags jansson)
CFLAGS += $(shell pkg-config --cflags libxdg-basedir)
CFLAGS += $(shell pkg-config --cflags lua)

LDFLAGS  = $(shell pkg-config --libs libcurl)
LDFLAGS += $(shell pkg-config --libs jansson)
LDFLAGS += $(shell pkg-config --libs libxdg-basedir)
LDFLAGS += $(shell pkg-config --libs lua)

PREFIX    ?= /usr
INSTALLDIR = $(DESTDIR)$(PREFIX)
BINDIR     = $(INSTALLDIR)/bin
MANDIR     = $(INSTALLDIR)/share/man/man1

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

$(MANPAGE): $(MANPAGE).ronn
	ronn -b --manual="HEKTOR MANUAL" $<

manpage: $(MANPAGE)

install: all
	install -D $(EXECUTABLE) $(BINDIR)/$(EXECUTABLE)
	install -D $(MANPAGE) $(MANDIR)/$(MANPAGE)

clean:
	rm $(OBJ)
