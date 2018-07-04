all: go-gtk-demo run

go-gtk-demo: go-gtk-demo.go go-gtk-demo.c
	go build -o go-gtk-demo .
run:
	G_ENABLE_DIAGNOSTIC=1 ./go-gtk-demo "Hi!" --class=GoGtkDemoClass "Ho!"
