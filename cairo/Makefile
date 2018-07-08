all: cairo run

cairo: cairo.go cairo.c my_cairo.h
	go build -o cairo .

run:
	G_ENABLE_DIAGNOSTIC=1 ./cairo
