//go:build gtk4

package main

/*
#cgo pkg-config: gtk4
#cgo CFLAGS: -DGDK_DISABLE_DEPRECATED -D_doesnt_work_with_webkit_GTK_DISABLE_DEPRECATED
#cgo LDFLAGS: -lm -rdynamic
*/
import "C"
