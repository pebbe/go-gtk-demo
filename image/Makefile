all: image run

image: image.go image.c my_image.h
	go build -o image .
run:
	G_ENABLE_DIAGNOSTIC=1 ./image
