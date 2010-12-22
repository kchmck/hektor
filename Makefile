EXECUTABLE = hektor

OBJS = config.o     \
       hektor.o     \
       hook.o       \
       info.o       \
       info-page.o  \
       lua-util.o   \
       modem.o      \
       path.o       \
       unit.o       \

LIBS = libcurl lua libxdg-basedir

STD_CFLAGS  = -pipe -std=c99 -Wall -Wno-switch -O2
STD_CFLAGS += $(shell pkg-config --cflags $(LIBS))

STD_LDFLAGS = $(shell pkg-config --libs $(LIBS))

ALL_CFLAGS  = $(STD_CFLAGS) $(CFLAGS)
ALL_LDFLAGS = $(STD_LDFLAGS) $(LDFLAGS)

PREFIX     = /usr
INSTALLDIR = $(DESTDIR)$(PREFIX)
BINDIR     = $(INSTALLDIR)/bin

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(ALL_LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(ALL_CFLAGS) -c $<

install: all
	install -D $(EXECUTABLE) $(BINDIR)/$(EXECUTABLE)

uninstall:
	rm -f $(BINDIR)/$(EXECUTABLE)

clean:
	rm $(OBJS)

.PHONY: all install uninstall clean
