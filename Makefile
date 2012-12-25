CC := g++
SFMLDIR := ./deps/SFML
LUADIR := ./deps/lua
LIBS := -L$(SFMLDIR)/lib -I$(SFMLDIR)/include -L$(LUADIR) -I$(LUADIR)
LINK := -llua52 -lsfml-graphics -lsfml-window -lsfml-system
CMD := $(CC) -std=c++11 $(LIBS) -g 

all: main.exe

main.o: main.cpp
	$(CMD) -c main.cpp -o main.o

desk.o: lib/desk.cpp
	$(CMD) -c lib/desk.cpp -o desk.o

bridge.o: lib/bridge.cpp
	$(CMD) -c lib/bridge.cpp -o bridge.o 

main.exe: main.o desk.o bridge.o
	$(CMD) main.o desk.o bridge.o -o main.exe $(LINK)

clean:
	rm -rf *.o *.exe

start:
	.\main

# vim: tabstop=4:softtabstop=4:shiftwidth=4:noexpandtab
