all: basic run

basic: basic.go basic.c my_basic.h
	go build -o basic .
run:
	G_ENABLE_DIAGNOSTIC=1 ./basic
