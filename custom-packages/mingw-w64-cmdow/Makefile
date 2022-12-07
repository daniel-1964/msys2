PREFIX?=/usr/local
CFG?=release
BINDIR=${CURDIR}/bin/${CFG}
OBJDIR=${CURDIR}/build/${CFG}
BIN=cmdow.exe

#CC=x86_64-w64-mingw32-gcc
#CXX=x86_64-w64-mingw32-g++
CXXFLAGS=-fno-threadsafe-statics -std=gnu++03
CPPFLAGS_debug=-g -O0
CPPFLAGS_release=-O3
CPPFLAGS=-Wall -Wextra -Werror -Wno-cast-function-type ${CPPFLAGS_${CFG}}
LDFLAGS_release=-s
LDFLAGS=${LDFLAGS_${CFG}}

all: mkdirs ${BINDIR}/${BIN}

INCS=	header.h
SRCS=	args.cpp \
	help.cpp \
	main.cpp \
	tlist.cpp \
	wlist.cpp
${SRCS}: ${INCS}
OBJS=${SRCS:%.cpp=%.o}

${OBJDIR}/%.o: %.cpp
	${COMPILE.cpp} $< -o $@

${BINDIR}/${BIN}: ${addprefix ${OBJDIR}/, ${OBJS}}
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

mkdirs:
	@test ! -d ${BINDIR} && mkdir -p ${BINDIR} || true
	@test ! -d ${OBJDIR} && mkdir -p ${OBJDIR} || true

clean:
	rm -f ${addprefix ${OBJDIR}/,${OBJS}} ${OBJDIR}/README.html ${BINDIR}/${BIN}

doc: ${OBJDIR}/README.html

${OBJDIR}/README.html: README.md ${wildcard cmdow-screenshot?.png} cmdow.cbp LICENSE.txt
	sed -e 's#https://github.com/ritchielawrence/cmdow/blob/master#.#g' \
	    -e 's#https://raw.githubusercontent.com/ritchielawrence/cmdow/master#.#g' \
	    README.md | \
	markdown_py.exe -o html -f ${OBJDIR}/README.html

install: ${BINDIR}/$(BIN) doc
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -p ${BINDIR}/${BIN} $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/doc/cmdow
	install -p ${wildcard cmdow-screenshot?.png} $(DESTDIR)$(PREFIX)/share/doc/cmdow
	install -p LICENSE.txt cmdow.cbp $(DESTDIR)$(PREFIX)/share/doc/cmdow
	install -p ${OBJDIR}/README.html $(DESTDIR)$(PREFIX)/share/doc/cmdow

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
	rm -fr $(DESTDIR)$(PREFIX)/share/doc/cmdow
