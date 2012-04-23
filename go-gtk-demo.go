package main

/*
#cgo pkg-config: gtk+-2.0
void init(void*, int*, char**);
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
	ln := len(os.Args)
	argc := C.int(ln)
	argv := make([]*C.char, ln)
	C.init(unsafe.Pointer(&os.Args[0]), &argc, &argv[0])

	// update os.Args
	ln = int(argc)
	os.Args = os.Args[0:0]
	for i := 0; i < ln; i++ {
		os.Args = append(os.Args, C.GoString(argv[i]))
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
