# learn how to use webfr:

- webfr is webframework that runs on low any devices

# Hello world:

- first program using webfr

```golang
// package should be main
package main

// import the module 
import (
    "github.com/krishpranav/webfr"
)

func main() {
    // new application
    wb := webfr.New()

    // path
    wb.Get("/hello", func(ctx webfr.Context) {
        ctx.SendString("Helo Friends!!")
    })

    // start the server
    wb.Start(":3000")
}
```