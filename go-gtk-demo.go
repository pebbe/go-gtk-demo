package main

/*
#cgo pkg-config: gtk+-2.0
#include <gtk/gtk.h>
void setup(void);
*/
import "C"

import (
	"fmt"
)

var (
	msgCh = make(chan msg, 100)
)

type msg struct {
	id int
	ms string
}

//export go_message
func go_message(id int, cContent *C.char) {
	content := C.GoString(cContent)
	msgCh <- msg{id: id, ms: content}
}

func main() {
	go C.setup()
	for {
		m := <-msgCh
		fmt.Println(m.ms)
		if m.id == 0 {
			break
		}
	}
}
