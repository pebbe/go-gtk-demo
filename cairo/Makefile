all: cairo run

cairo: cairo.go cairo.c cairo_my.h
	go build -o cairo .

run:
	G_ENABLE_DIAGNOSTIC=1 ./cairo
