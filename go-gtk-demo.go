package main

/*
#cgo pkg-config: gtk+-2.0
#include <gtk/gtk.h>
void setup();
*/
import "C"

import (
	"fmt"
	"strings"
)

var (
	msgCh = make(chan msg, 100)
)

type msg struct {
	id int
	ms string
}

//export go_message
func go_message(id int, content string) {
	// make a copy before sending 'content' over a channel, because once
	// this function finishes, the string may be delete by the calling C code
	s2 := strings.Repeat(content, 1)
	msgCh <- msg{id: id, ms: s2}
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
