package main

/*
#cgo pkg-config: gtk+-2.0
#include <gtk/gtk.h>
#include <stdlib.h>
char *return_arg(int i);
int init(int argc, char **argv);
void setup(void);
*/
import "C"

import (
	"flag"
	"fmt"
	"os"
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
	argc := len(os.Args)
	argv := make([]*C.char, 0, argc)
	for _, arg := range os.Args {
		argv = append(argv, C.CString(arg))
	}
	argc = int(C.init(C.int(argc), &argv[0]))

	// clean-up
	for _, a := range argv {
		C.free(unsafe.Pointer(a))
	}

	// update os.Args
	os.Args = os.Args[0:0]
	for i := 0; i < argc; i++ {
		os.Args = append(os.Args, C.GoString(C.return_arg(C.int(i))))
	}

	fmt.Println("Remaining command line arguments:", os.Args)
}

func main() {

	initGTK()

	flag.Parse()
	fmt.Println("Remaining flag.Args():", flag.Args())

	go C.setup()

	for {
		m := <-msgCh
		fmt.Println(m.ms)
		if m.id == 0 {
			break
		}
	}
}
