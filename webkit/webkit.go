package main

/*
#cgo pkg-config: gtk+-3.0 webkit2gtk-4.0
#cgo CFLAGS: -DGDK_DISABLE_DEPRECATED -D_doesnt_work_with_webkit_GTK_DISABLE_DEPRECATED
#include "webkit_my.h"
*/
import "C"

import (
	"log"
	"runtime"
)

type msg struct {
	id int
	ms string
}

var (
	chGtkDone = make(chan bool)
	chGoDone  = make(chan bool)
	chMessage = make(chan msg, 100)
)

//export go_message
func go_message(id int, cContent *C.char) {
	content := C.GoString(cContent)
	chMessage <- msg{id: id, ms: content}
}

func main() {

	go doStuff()

	runtime.LockOSThread()

	C.run()
	log.Println("Gtk done")
	close(chGtkDone)

	<-chGoDone
	log.Println("All done")
}

func doStuff() {
	defer close(chGoDone)

	// Wait for Gtk to get ready
LOOP:
	for {
		select {
		case <-chGtkDone:
			return
		case m := <-chMessage:
			doMessage(m)
			if m.id == C.idREADY {
				break LOOP
			}
		}
	}

	for {
		select {
		case <-chGtkDone:
			return
		case m := <-chMessage:
			doMessage(m)
		}
	}
}

func doMessage(m msg) {
	switch m.id {
	default:
		log.Printf("-- unknown message id %d: %q\n", m.id, m.ms)
	case C.idERROR:
		log.Printf("-- error: %s\n", m.ms)
	case C.idREADY:
		log.Printf("-- ready: %s\n", m.ms)
	case C.idDELETE:
		log.Printf("-- delete event: %s\n", m.ms)
	case C.idDESTROY:
		log.Printf("-- destroy event: %s\n", m.ms)
	case C.idLOADED:
		log.Printf("-- page loaded: %s\n", m.ms)
	}
}
