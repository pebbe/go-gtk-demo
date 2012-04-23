package main

/*
#cgo pkg-config: gtk+-2.0
char *return_arg(int);
int init(int, void*);
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
	argc := int(C.init(C.int(len(os.Args)), unsafe.Pointer(&os.Args[0])))

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
