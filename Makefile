EXECUTABLE = hektor
OBJ = hektor-fap.o            \
      hektor-modem.o          \
      hektor-units.o          \
      hektor.o

LIBS = libcurl

STD_CFLAGS  = -pipe -std=c99 -Wall -O2
STD_CFLAGS += $(shell pkg-config --cflags $(LIBS))

STD_LDFLAGS = $(shell pkg-config --libs $(LIBS))

ALL_CFLAGS  = $(STD_CFLAGS) $(CFLAGS)
ALL_LDFLAGS = $(STD_LDFLAGS) $(LDFLAGS)

PREFIX     = /usr
INSTALLDIR = $(DESTDIR)$(PREFIX)
BINDIR     = $(INSTALLDIR)/bin

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(ALL_LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(ALL_CFLAGS) -c $<

install: all
	install -D $(EXECUTABLE) $(BINDIR)/$(EXECUTABLE)

uninstall:
	rm -f $(BINDIR)/$(EXECUTABLE)

clean:
	rm $(OBJ)

.PHONY: all install uninstall clean
