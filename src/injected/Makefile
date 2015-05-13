CC:=LANG=C i686-w64-mingw32-gcc -Wall -g -Werror -std=c99

# Automatic C dependency detection (just gcc, no make recursion etc)
DEPS=$(shell $(CC) -MM $(1) | cut -d ':' -f 2 | tr -d '\n\\')
OBJS=$(shell grep obj/ Makefile | grep 'call DEPS' | grep -v OBJS | cut -d':' -f 1 | tr '\n' ' ')

all: obj ../../bin ../../bin/proxy.dll


# PROXY.DLL
../../bin/proxy.dll: $(OBJS)
	$(CC) -shared -o $@ obj/*.o -lws2_32

obj/proxy.o: $(call DEPS,proxy.c)
	$(CC) -c -o $@ $<

# Thread (cpp)
obj/thread.o: $(call DEPS,thread/thread.c)
	$(CC) -c -o $@ $<


# Server (currently not used!)
obj/server.o: $(call DEPS,server/injected_server.c)
	$(CC) -c -o $@ $<

# Network
obj/net.o: $(call DEPS,net/injected_net.c)
	$(CC) -c -o $@ $<


# OTHER
obj ../../bin:
	mkdir -p $@

clean:
	rm ../../bin/proxy.dll || true

.PHONY: all clean
