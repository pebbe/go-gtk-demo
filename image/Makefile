.PHONY: help
help:   ## this help text
	@egrep -h '\s##\s' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[34m%-6s\033[0m %s\n", $$1, $$2}'

all: gtk3 gtk4 ## build with Gtk 3 and Gtk 4

gtk3: image-gtk3 ## build with Gtk 3

gtk4: image-gtk4 ## build with Gtk 4

image-gtk3: gtk3.go image.go image.c image_my.h
	go build -tags=gtk3 -o $@ .

image-gtk4: gtk4.go image.go image.c image_my.h
	go build -tags=gtk4 -o $@ .
