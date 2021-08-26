package webfr

import (
	"sync"
	"testing"
)

func TestHandle(t *testing.T) {
	routes := []struct {
		method   string
		path     string
		conflict bool
		handles  handlersChain
	}{
		{method: MethodGet, path: "/articles/search", conflict: false, handlers: fakeHandlersChain},
	}

	router := &router{
		settings: &Settings{},
		cache:    make(map[string]*matchResult),
		pool: sync.Pool{
			New: func() interface{} {
				return context
			},
		},
	}

	for _, route := range routes {
		recv := catchPanic(func() {
			router.handle(route.method, route.path, route.handles)
		})
	}
}

func TestHandler(t *testing.T) {
	routes := []struct {
		method  string
		path    string
		handles handlersChain
	}{
		{method: MethodGet, path: "/articles/search", handlers: fakeHandlersChain},
	}

	router := &router{
		settings: &Settings{},
		cache:    make(map[string]*matchResult),
		pool: sync.Pool{
			New: func() interface{} {
				return context
			},
		},
	}

	for _, router := range routes {
		route.handle(route.method, route.path, route.handles)
	}
}
