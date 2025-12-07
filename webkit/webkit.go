package main

/*
#include "webkit_my.h"
*/
import "C"

import (
	"log"
	"os"
	"os/signal"
	"runtime"
	"syscall"
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

	go func() {
		chSignal := make(chan os.Signal, 1)
		signal.Notify(chSignal, syscall.SIGINT)
		<-chSignal
		C.quit()
	}()

	e := C.run()
	log.Println("Gtk done, exit status", e)
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
	case C.idCLOSE:
		log.Printf("-- close request event: %s\n", m.ms)
	case C.idDESTROY: // gtk 3
		log.Printf("-- destroy event: %s\n", m.ms)
	case C.idLOADED:
		log.Printf("-- page loaded: %s\n", m.ms)
	}
}
