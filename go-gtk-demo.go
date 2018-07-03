package main

/*
#cgo pkg-config: gtk+-3.0
#include <stdlib.h>
char *return_arg(int);
int init(int, void*);
void run(void);
void update_button(char *text);
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

	fmt.Println("Done")
}

func doStuff() {
	time.Sleep(100 * time.Millisecond)

	ticker := time.Tick(1000 * time.Millisecond)
LOOP:
	for {
		select {
		case m := <-msgCh:
			fmt.Println(m.ms)
			if m.id == 0 {
				break LOOP
			}
		case <-ticker:
			s := C.CString(fmt.Sprintf("Hello World at %s", time.Now().Format("3:04:05")))
			C.update_button(s)
			// s is freed in C
		}
	}
}
