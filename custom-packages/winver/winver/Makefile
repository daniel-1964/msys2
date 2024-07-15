PREFIX?=/usr/local
CPPFLAGS=-std=c++11 -pthread -O3 -g -Wall -Wextra -Werror -D_WIN32_WINNT=0x0604 -Wno-cast-function-type
#LDFLAGS?=-static-libgcc -static-libstdc++ -static
#CXX=x86_64-w64-mingw32-g++
OBJDIR=$(CURDIR)/build
EXEDIR=$(CURDIR)/bin
SRCDIR=$(CURDIR)
BIN=$(EXEDIR)/winver.exe
SRCS=$(wildcard $(CURDIR)/*.cpp)
OBJS=$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRCS:%.cpp=%.o))

all: mkdir $(BIN)

mkdir:
	@-mkdir $(OBJDIR) $(EXEDIR)

$(BIN): $(OBJS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(SRCS): $(SRCDIR)/SystemInfo.h

clean:
	rm -f $(OBJS) $(BIN)
	rmdir $(EXEDIR) $(OBJDIR)

install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -p $(BIN) $(DESTDIR)$(PREFIX)/bin

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
