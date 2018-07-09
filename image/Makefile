all: image run

image: image.go image.c image_my.h
	go build -o image .
run:
	G_ENABLE_DIAGNOSTIC=1 ./image
