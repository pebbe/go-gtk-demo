package main

/*
#cgo pkg-config: gtk+-3.0 webkit2gtk-4.0
#cgo CFLAGS: -DGDK_DISABLE_DEPRECATED -D_doesnt_work_with_webkit_GTK_DISABLE_DEPRECATED
#include "go-gtk-demo.h"
*/
import "C"

import (
	"flag"
	"fmt"
	"log"
	"os"
	"runtime"
	"time"
	"unsafe"
)

var (
	chMessage = make(chan msg, 100)
	chGtkQuit = make(chan bool)
	chGoQuit  = make(chan bool)
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

func initGTK() {

	// pass command line arguments to GTK+
	argc := int(C.init(C.int(len(os.Args)), unsafe.Pointer(&os.Args[0])))

	// update os.Args
	os.Args = os.Args[0:0]
	for i := 0; i < argc; i++ {
		os.Args = append(os.Args, C.GoString(C.return_arg(C.int(i))))
	}

	fmt.Println("Remaining command line arguments:", os.Args)
}

func main() {

	go doStuff()

	runtime.LockOSThread()

	initGTK()

	flag.Parse()
	fmt.Println("Remaining flag.Args():", flag.Args())

	C.run()
	fmt.Println("Gtk done")
	close(chGtkQuit)

	<-chGoQuit
	fmt.Println("All done")
}

func doStuff() {
	ticker1 := time.Tick(1000 * time.Millisecond)
	ticker2 := time.Tick(4567 * time.Millisecond)
LOOP:
	for {
		select {
		case <-chGtkQuit:
			break LOOP
		case m := <-chMessage:
			doMessage(m)
		case <-ticker1:
			s := C.CString(time.Now().Format("3:04:05"))
			C.update_label(s)
			// s is freed in C
		case <-ticker2:
			C.get_text()
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
	case C.idBUTTON:
		log.Printf("-- button clicked: %s\n", m.ms)
	case C.idTEXT:
		log.Printf("-- text received: %q\n", m.ms)
	}
}
