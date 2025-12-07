//go:build !gtk4
// +build !gtk4

package main

/*
#cgo pkg-config: gtk+-3.0
#cgo CFLAGS: -DGDK_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
#cgo LDFLAGS: -rdynamic
*/
import "C"
