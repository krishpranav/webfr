package webfr

import (
	jsoniter "github.com/json-iterator/go"
	"github.com/valyala/fasthttp"
)

type Context interface {
	Next()
	Body() string
	Param(key string) string
	Query(key string) string
	SendBytes(value []byte) Context
	GetLocal(key string) interface{}
}

const (
	MIMEApplicationJSON = "application/json"
)

type handlerFunc func(ctx Context)

type handlersChain []handlerFunc

type context struct {
	requestCtx  *fasthttp.RequestCtx
	paramValues map[string]string
	handlers    handlersChain
	index       int
}

func (ctx *context) Next() {
	ctx.index++
	if ctx.index < len(ctx.handlers) {
		ctx.handlers[ctx.index](ctx)
	}
}

func (ctx *context) GetLocal(key string) interface{} {
	return ctx.requestCtx.UserValue(key)
}

func (ctx *context) SendJSON(in interface{}) error {
	json := jsoniter.ConfigCompatibleWithStandardLibrary
	raw, err := json.Marshal(in)

	if err != nil {
		return err
	}

	ctx.requestCtx.Response.Header.SetContentType(MIMEApplicationJSON)
	ctx.requestCtx.Response.SetBodyRaw(raw)

	return nil
}
