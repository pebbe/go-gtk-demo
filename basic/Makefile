all: basic run

basic: basic.go basic.c basic_my.h
	go build -o basic .
run:
	G_ENABLE_DIAGNOSTIC=1 ./basic
