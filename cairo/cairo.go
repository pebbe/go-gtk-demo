package main

/*
#cgo pkg-config: gtk+-3.0
#cgo CFLAGS: -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
#cgo LDFLAGS: -lm
#include "my_cairo.h"
*/
import "C"

import (
	"fmt"
	"log"
	"runtime"
	"time"
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
	ticker := time.Tick(1000 * time.Millisecond)
LOOP:
	for {
		select {
		case <-chGtkQuit:
			break LOOP
		case m := <-chMessage:
			doMessage(m)
		case <-ticker:
			now := time.Now()
			C.update_image(C.int(now.Hour()), C.int(now.Minute()), C.int(now.Second()))
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
