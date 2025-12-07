.PHONY: help
help:   ## this help text
	@egrep -h '\s##\s' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[34m%-6s\033[0m %s\n", $$1, $$2}'

all: gtk3 gtk4 ## build with Gtk 3 and Gtk 4

gtk3: basic-gtk3 ## build with Gtk 3

gtk4: basic-gtk4 ## build with Gtk 4

basic-gtk3: gtk3.go basic.go basic.c basic_my.h
	go build -tags=gtk3 -o $@ .

basic-gtk4: gtk4.go basic.go basic.c basic_my.h
	go build -tags=gtk4 -o $@ .
