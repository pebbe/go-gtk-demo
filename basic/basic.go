package main

/*
#cgo pkg-config: gtk+-3.0
#cgo CFLAGS: -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
#cgo LDFLAGS: -rdynamic
#include "basic_my.h"
*/
import "C"

import (
	"log"
	"runtime"
	"time"
	"unsafe"
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

	setClock()
	ticker1 := time.Tick(1000 * time.Millisecond)
	ticker2 := time.Tick(4567 * time.Millisecond)
	for {
		select {
		case <-chGtkDone:
			return
		case m := <-chMessage:
			doMessage(m)
		case <-ticker1:
			setClock()
		case <-ticker2:
			C.get_text()
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
	case C.idBUTTON:
		log.Printf("-- button clicked: %s\n", m.ms)
	case C.idTEXT:
		log.Printf("-- text received: %q\n", m.ms)
	}
}

func setClock() {
	b := []byte(time.Now().Format("3:04:05"))
	b = append(b, 0)
	C.update_label(unsafe.Pointer(&b[0]))
}
