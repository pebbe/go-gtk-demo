package main

/*
#cgo pkg-config: gtk+-3.0
#cgo CFLAGS: -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
#include "my_image.h"
*/
import "C"

import (
	"io/ioutil"
	"log"
	"runtime"
	"unsafe"
)

type msg struct {
	id int
	ms string
}

var (
	chGtkQuit = make(chan bool)
	chGoQuit  = make(chan bool)
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
	close(chGtkQuit)

	<-chGoQuit
	log.Println("All done")
}

func doStuff() {
	defer close(chGoQuit)

	// Wait for Gtk to get ready
LOOP:
	for {
		select {
		case <-chGtkQuit:
			return
		case m := <-chMessage:
			doMessage(m)
			if m.id == C.idREADY {
				break LOOP
			}
		}
	}

	setImage()

	for {
		select {
		case <-chGtkQuit:
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
	}
}

func setImage() {
	data, err := ioutil.ReadFile("image.png")
	if err == nil {
		C.update_image(unsafe.Pointer(&data[0]), C.int(len(data)))
	} else {
		log.Println("Loading image file:", err)
	}
}
