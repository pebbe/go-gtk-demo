all: program run

program: program.go program.c program_my.h
	go build -o program .
run:
	G_ENABLE_DIAGNOSTIC=1 ./program
