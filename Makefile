PROGRAM=waiter.exe

SRCS=waiter.cpp

LIBS=user32.lib

LD=cl -nologo
LDFLAGS=-MD -O2

all: $(PROGRAM)

$(PROGRAM): $(SRCS)
	$(LD) $(LDFLAGS) -Fe$(PROGRAM) $(SRCS) $(LIBS)
