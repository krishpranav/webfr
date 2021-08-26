# webfr

[![forthebadge](https://forthebadge.com/images/badges/made-with-go.svg)](https://forthebadge.com)

- A powerful go web framework for highly scalable and resource efficient web application

# Installation:
```
go get -u github.com/krishpranav/webfr
```

# Examples:
```golang
package main

import (
    "github.com/krishpranav/webfr"
)

func main() {
    wb := webfr.New()

    wb.Get("/hello", func(ctx webfr.Context) {
        ctx.SendString("Helo Friends!!")
    })

    wb.Start(":3000")
}
```