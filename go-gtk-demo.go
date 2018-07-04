package main

/*
#cgo pkg-config: gtk+-3.0
#cgo CFLAGS: -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
#include <stdlib.h>
char *return_arg(int);
int init(int, void*);
void run(void);
void update_label(char *text);
void get_text(void);
*/
import "C"

import (
	"flag"
	"fmt"
	"os"
	"runtime"
	"time"
	"unsafe"
)

var (
	msgCh  = make(chan msg, 100)
	quitCh = make(chan bool)
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

	<-quitCh

	fmt.Println("Done")
}

func doStuff() {
	ticker1 := time.Tick(1000 * time.Millisecond)
	ticker2 := time.Tick(4567 * time.Millisecond)
LOOP:
	for {
		select {
		case m := <-msgCh:
			fmt.Println(m.ms)
			if m.id == 0 {
				break LOOP
			}
		case <-ticker1:
			s := C.CString(time.Now().Format("3:04:05"))
			C.update_label(s)
			// s is freed in C
		case <-ticker2:
			C.get_text()
		}
	}
	close(quitCh)
}
