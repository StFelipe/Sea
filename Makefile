name=game.exe

CC=gcc

# REMOVE -g FLAG FOR DEBUGGING WHEN BUILDING A RELEASE
CFLAGS=-IC:/mingw_link/mingw_include -Iengine
CFLAGS+=-g
#CFLAGS+=-MMD

LDLIBS=-LC:/mingw_link/mingw_lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

LDFLAGS_debug="-Wl,-subsystem,console"
LDFLAGS_release="-Wl,-subsystem,windows"
LDFLAGS=$(LDFLAGS_debug) 

SRCS=$(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c) $(wildcard */*/*/*.c)
OBJS=$(SRCS:.c=.o)
#DEPS=$(OBJS:.o=.d)

.PHONY: all clean
all: $(name)

$(name): $(OBJS)
	$(CC) -o $(name) $(OBJS) $(LDLIBS) $(LDFLAGS)

#-include $(DEPS)

#rm -f $(name)
#rm -f $(DEPS) $(OBJS)
#Del "%variable%"
#Del $(name)
clean:
	del /S *.o
	del $(win_name)