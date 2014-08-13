#
# Makefile for libulz (stolen from musl) (requires GNU make)
#
# Use config.mak to override any of the following variables.
# Do not make changes here.
#

prefix = /usr/local/
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
includedir = $(prefix)/include
libdir = $(prefix)/lib

SRCS = $(sort $(wildcard src/*/*.c))
OBJS = $(SRCS:.c=.o)
LOBJS = $(OBJS:.o=.lo)

CFLAGS += -Os
CFLAGS_REQ=-std=c99

#-ffreestanding -std=c99 -D_XOPEN_SOURCE=700 -pipe
#LDFLAGS = -nostdlib -shared -fPIC -Wl,-e,_start -Wl,-Bsymbolic-functions
INC     = -I./include
#PIC     = -fPIC -O3
AR      = $(CROSS_COMPILE)ar
RANLIB  = $(CROSS_COMPILE)ranlib
OBJCOPY = $(CROSS_COMPILE)objcopy

ALL_INCLUDES = $(sort $(wildcard include/*.h include/*/*.h))

ULZ_LIBS = lib/libulz.a
ALL_LIBS = $(ULZ_LIBS) 

-include config.mak

CFLAGS += $(CFLAGS_REQ)

all: $(ALL_LIBS)

install: $(ALL_LIBS:lib/%=$(DESTDIR)$(libdir)/%) $(ALL_INCLUDES:include/%=$(DESTDIR)$(includedir)/ulz/%) $(ALL_TOOLS:tools/%=$(DESTDIR)$(bindir)/%)

clean:
	rm -f crt/*.o
	rm -f $(OBJS)
	rm -f $(LOBJS)
	rm -f $(ALL_LIBS) lib/*.[ao] lib/*.so

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

lib/libulz.a: $(OBJS)
	rm -f $@
	$(AR) rc $@ $(OBJS)
	$(RANLIB) $@

lib/%.o:
	cp $< $@

$(DESTDIR)$(includedir)/ulz%: include/%
	install -D $< $@

$(DESTDIR)$(bindir)/%: tools/%
	install -D $< $@

$(DESTDIR)$(prefix)/%: %
	install -D -m 644 $< $@

.PHONY: all clean install
