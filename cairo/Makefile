.PHONY: help
help:   ## this help text
	@egrep -h '\s##\s' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[34m%-6s\033[0m %s\n", $$1, $$2}'

all: gtk3 gtk4 ## build with Gtk 3 and Gtk 4

gtk3: cairo-gtk3 ## build with Gtk 3

gtk4: cairo-gtk4 ## build with Gtk 4

cairo-gtk3: gtk3.go cairo.go cairo.c cairo_my.h
	go build -tags=gtk3 -o $@ .

cairo-gtk4: gtk4.go cairo.go cairo.c cairo_my.h
	go build -tags=gtk4 -o $@ .
