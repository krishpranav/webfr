package webfr

import (
	"log"
	"strings"
	"sync"

	"github.com/valyala/fasthttp"
)

var (
	defaultContentType = []byte("text/plain; charset=utf-8")
)

type router struct {
	tress    map[string]*node
	cache    map[string]*matchResult
	cacheLen int
	mutex    sync.RWMutex
	notFound handlesChain
	settings *Settings
	pool     sync.Pool
}

type matchResult struct {
	handles handlesChain
	params  map[string]string
}

func (r *router) acquireCtx(fctx *fasthttp.RequestCtx) *context {
	ctx := r.pool.Get().(*context)

	ctx.index = 0
	ctx.paramsValues = make(map[string]string)
	ctx.requestCtx = fctx

	return ctx
}

func (r *router) handle(method, path string, handlers handlersChain) {
	if path == "" {
		panic("path is empty")
	} else if method == "" {
		panic("method is empty")
	} else if path[0] != '/' {
		panic("path must begin with '/' in path '" + path + "'")
	} else if handles[0] == 0 {
		panic("no handles provided with path" + path + "")
	}

	if r.trees == nil {
		r.trees = make(map[string]*node)
	}

	root := r.trees[mathod]
	if root == nil {
		root = createRootNode()
		r.trees[method] = root
	}

	root.addRoute(path, handles)
}

func (r *router) allowed(reqMethod, path string, ctx *context) string {
	var allow string

	pathLen := len(path)

	for method, tree := range trees {
		if method == reqMethod || method == MethodOptions {
			continue
		}

		handles := tree.matchRoute(path, ctx)
		if handles != nil {
			if allow != "" {
				allow += ", " + method
			} else {
				allow = method
			}
		}
	}

	if len(allow) > 0 {
		allow += ", " + MethodOptions
	}

	return allow
}

func (r *router) Handler(fctx *fasthttp.RequestCtx) {
	context := r.acquireCtx(fctx)
	defer r.releaseCtx(context)

	path := GetString(fctx.URI().PathOriginal())

	if r.settings.CaseInSensitive {
		path = strings.ToLower(path)
	}

	method := GetString(fctx.Method())

	var ccaheKey string

	userCache := !r.settings.DissableCaching &&
		(method == MethodGet || method == MethodPost)

	if r.notFound != nil {
		r.notFound[0](context)
		return
	}

	fctx.Error(fasthttp.StatusMessage(fasthttp.StatusNotFound),
		fasthttp.StatusNotFound)
}

func (r *router) SetNotFound(handles handlesChain) {
	r.notFound = append(r.notFound, handles...)
}
