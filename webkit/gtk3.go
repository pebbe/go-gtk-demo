//go:build !gtk4
// +build !gtk4

package main

/*
#cgo pkg-config: gtk+-3.0 webkit2gtk-4.0
#cgo CFLAGS: -DGDK_DISABLE_DEPRECATED -D_doesnt_work_with_webkit_GTK_DISABLE_DEPRECATED
#cgo LDFLAGS: -rdynamic
*/
import "C"
