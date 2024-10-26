CSOURCES = main.c util.c
SSOURCES = conio.s

PROGRAM = eprom

CC65_TARGET = none
CFLAGS      = -t $(CC65_TARGET) -O --cpu 6502 --standard cc65
CAFLAGS     = --cpu 6502
LDFLAGS     = -C apple1.ld65 -m $(PROGRAM).map

.SUFFIXES:
.PHONY: all clean

all: $(PROGRAM)

#ifneq ($(MAKECMDGOALS),clean)
#-include $(SOURCES:.c=.d)
#endif

%.s: %.c
	cc65 $(CFLAGS) -o $@ $<

%.o: %.s
	ca65 $(CAFLAGS) -o $@ $<

$(PROGRAM): $(CSOURCES:.c=.o) $(SSOURCES:.s=.o)
	ld65 $(LDFLAGS) -o $@ $^ apple1.lib

#clean:
#        $(RM) $(SOURCES:.c=.o) $(SOURCES:.c=.d) $(PROGRAM) $(PROGRAM).map

# --create-dep $(<:.c=.d) -O