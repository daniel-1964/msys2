PREFIX?=/usr/local
CPPFLAGS=-O3 -g -Wall -Wextra -Werror -D_WIN32_WINNT=0x0604 -Wno-cast-function-type
#CXX=x86_64-w64-mingw32-g++
BIN=winver.exe
OBJS=SystemInfo.o \
     winver.o

all: $(BIN)

$(BIN): $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

winver.o: winver.cpp SystemInfo.h
SystemInfo.o: SystemInfo.cpp SystemInfo.h

clean:
	rm -f $(OBJS) $(BIN)

install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -p $(BIN) $(DESTDIR)$(PREFIX)/bin

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
