# learn how to use webfr:

- webfr is webframework that runs on any devices

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

- command to run the server:
```
go run main.go
```

- navigate to ```127.0.0.1/hello```

- more tutorials will be added soon!!!!