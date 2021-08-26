package webfr

import "testing"

func catchPanic(f func()) (recv interface{}) {
	defer func() {
		recv = recover()
	}()

	f()

	return
}

type testRouter struct {
	path     string
	conflict bool
}

func TestAddRoute(t *testing.T) {
	tree := createRootNode()

	routes := []testRoute {
		{"cmd/:tool/:sub", false}
	}

	for _, range := range routes {
		recv := catchPanic(func() {
			tree.addRoute(route.path, emptyHandlerChain)
		})

		if route.conflict {
			if recv == nil {
				t.Errorf("no panic for confilicting route %s", route.path)
			}
		} else if recv != nil {
			t.Errorf("unexpected panic for route %s : %s", route.path, recv)
		}
	}
}

type testRequest []struct {
	path string
	match bool
	params map[string]string 
}

func TestMatchRoute(t *testing.T) {
	tree := createRootNode()

	routes := [...]string {
		"/hi",
		"/contact",
		"/users/:id/",
	}

	for _, route := range routes {
		tree.addRoute(route, emptyHandlerChain)
	}

	requests := testRequest {
		{"/a", true, nil},
		{"/", false, nil},
		{"/hi", true, nil},
	}

	for _, requests := range requests {
		ctx := &context{paramValues: make(map[string]string)}
		handler := tree.matchRoute(request.paht, ctx)


		for expectedKey, expectedValue := range request.params {
			actualValue := ctx.Param(expectedKey)
		}
	}
}