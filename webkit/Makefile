all: webkit run

webkit: webkit.go webkit.c my_webkit.h
	go build -o webkit .
run:
	G_ENABLE_DIAGNOSTIC=1 ./webkit
