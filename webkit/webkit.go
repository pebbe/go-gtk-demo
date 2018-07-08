package main

/*
#cgo pkg-config: gtk+-3.0 webkit2gtk-4.0
#cgo CFLAGS: -DGDK_DISABLE_DEPRECATED -D_doesnt_work_with_webkit_GTK_DISABLE_DEPRECATED
#include "my_webkit.h"
*/
import "C"

import (
	"fmt"
	"log"
	"runtime"
)

var (
	chGtkQuit = make(chan bool)
	chGoQuit  = make(chan bool)

	chMessage = make(chan msg, 100)
)

type msg struct {
	id int
	ms string
}

//export go_message
func go_message(id int, cContent *C.char) {
	content := C.GoString(cContent)
	chMessage <- msg{id: id, ms: content}
}

func main() {

	go doStuff()

	runtime.LockOSThread()

	C.run()
	fmt.Println("Gtk done")
	close(chGtkQuit)

	<-chGoQuit
	fmt.Println("All done")
}

func doStuff() {
LOOP:
	for {
		select {
		case <-chGtkQuit:
			break LOOP
		case m := <-chMessage:
			doMessage(m)
		}
	}
	close(chGoQuit)
}

func doMessage(m msg) {
	switch m.id {
	default:
		log.Printf("-- unknown message id %d: %q\n", m.id, m.ms)
	case C.idERROR:
		log.Printf("-- error: %s\n", m.ms)
	case C.idDELETE:
		log.Printf("-- delete event: %s\n", m.ms)
	case C.idDESTROY:
		log.Printf("-- destroy event: %s\n", m.ms)
	}
}
