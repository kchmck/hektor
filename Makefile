OBJ = hektor-config.o         \
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

CFLAGS += -std=c99 -Wall -O2
CFLAGS += $(shell pkg-config --cflags libcurl)
CFLAGS += $(shell pkg-config --cflags jansson)
CFLAGS += $(shell pkg-config --cflags libxdg-basedir)
CFLAGS += $(shell pkg-config --cflags lua)

LDFLAGS  = $(shell pkg-config --libs libcurl)
LDFLAGS += $(shell pkg-config --libs jansson)
LDFLAGS += $(shell pkg-config --libs libxdg-basedir)
LDFLAGS += $(shell pkg-config --libs lua)

PREFIX ?= /usr/local
BINDIR  = $(PREFIX)/bin

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

install: all
	install -Ds $(EXECUTABLE) $(DESTDIR)$(BINDIR)/$(EXECUTABLE)

clean:
	rm $(OBJ)
