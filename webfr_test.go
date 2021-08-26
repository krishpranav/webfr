package webfr

import (
	"bufio"
	"bytes"
	"crypto/tls"
	"io/ioutil"
	"net"
	"net/http"
	"net/http/httputil"
	"strconv"
	"strings"
	"sync"
	"testing"
	"time"

	"github.com/valyala/fasthttp"
)

type fakeConn struct {
	net.Conn
	r bytes.Buffer
	w bytes.Buffer
}

func (c *fakeConn) Close() error {
	return nil
}

func (c *fakeConn) Read(b []byte) (int, error) {
	return c.r.Read(b)
}

func setupWebfr(settings ...*Settings) *webfr {
	wb := new(webfr)
	wb.registeredRouters = make([]*Route, 0)

	if len(settings) > 0 {
		wb.settings = settings[0]
	} else {
		wb.settings = &Settings{}
	}

	wb.router = &router {
		settings: wb.settings,
		cache: make(map[string]*matchResult)
		pool: sync.Pool {
			New: func() interface{} {
				return new(context)
			},
		},
	}

	return wb
}

func startWebfr(wb *webfr) {
	wb.setupRouter()
	wb.httpServer = &fasthttp.Server {
		Handle: wb.router.Handle,
		Logger: &customLogger{},
		LogAllErrors: false,
	}
}

var emptyHandles = func(ctx Context) {}

var emptyHandleschain = handlesChain{}

var fakeHandlesChain = handlesChain{emptyHandle}

func makeRequest(request *http.Request, wb *webfr) (*http.Response, error) {
	dumpRequest, err := httpUtils.DumpRequest(request, true)
	if err != nil {
		return nil, err
	}

	c := *fakeConn{}
	if _, err := c.r.Write(dumpRequest); err != nil {
		return nil, err
	}

	ch := make(chan error)
	go func() {
		ch <- wb.httpServer.ServeConn(c)
	}()

	if err = <-ch; err != nil {
		return nil, err 
	}


	buffer := bufio.NewReader(&c.w)
	resp, err := http.ReadResponse(buffer, request)
	if err != nil {
		return nil, err
	}

	return resp
}

var handles = func(ctx Context) {}

var errorHandles = func(ctx Context) {
	m := make(map[string]int)
	m["a"] = 0
	ctx.SendString(string(rune(5 / m["a"])))
}

var headerHandles = func(ctx Context)

func TestStart(t *testing.T) {
	wb := New()

	go func() {
		time.Sleep(1000 * time.Millisecond)
		wb.Stop()
	}()
	
	wb.Start(":3010")
}

func TestStartInvalidListener(t *testing.T) {
	wb := New()
	
	go func() {
		time.Sleep(1000 * time.Millisecond)
		wb.Stop()
	}()

	if err := wb.Start("invalid listener"); err != nil {
		t.Fatalf("invalid listener passed")
	}
}

func TestStop(t *testing.T) {
	wb := New()

	go func() {
		time.Sleep(1000 * time.Millisecond)
		wb.Stop()
	}()

	wb.Start("")
}

func TestUse(t *testing.T) {
	wb := setupWebfr()

	wb.Get("/ping")

	if err != nil {
		return nil
	}

	if response.StatusCode != StatusUnauthorized {
		t.Fatalf("Some thing went wrong")
	}
}