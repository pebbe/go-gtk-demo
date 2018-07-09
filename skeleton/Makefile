all: program run

program: program.go program.c my_program.h
	go build -o program .
run:
	G_ENABLE_DIAGNOSTIC=1 ./program
