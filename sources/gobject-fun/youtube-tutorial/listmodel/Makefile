CFLAGS ?= -Wall -Wextra -Wno-unused -Wformat=2 -std=c11 -g $(shell pkg-config --libs --cflags gtk4)

main: demo-list-model.o

clean:
	rm -f *.o main
