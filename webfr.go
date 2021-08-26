package webfr

import (
	"log"
	"net"
	"os"
	"strings"
	"sync"
	"time"

	"github.com/valyala/fasthttp"
	"github.com/valyala/fasthttp/prefork"
)

const (
	Version = "1.0.0"
	Name    = "webfr"

	banner = `
	WEB FRAMEWORK

Listening on %s`
)

const (
	defaultCacheSize           = 1000
	defaultConcurrency         = 256 * 1024
	defaultMaxRequestBodySize  = 4 * 1024 * 1024
	defaultMaxRouteParams      = 1024
	defaultMaxRequestURLLength = 2048
)

const (
	MethodGet     = "GET"
	MethodHead    = "HEAD"
	MethodPost    = "POST"
	MethodPut     = "PUT"
	MethodPatch   = "PATCH"
	MethodDelete  = "DELETE"
	MethodConnect = "CONNECT"
	MethodOptions = "OPTIONS"
	MethodTrace   = "TRACE"
)

const (
	StatusContinue           = 100
	StatusSwitchingProtocols = 101
	StatusProcessing         = 102

	StatusOK                   = 200
	StatusCreated              = 201
	StatusAccepted             = 202
	StatusNonAuthoritativeInfo = 203
	StatusNoContent            = 204
	StatusResetContent         = 205
	StatusPartialContent       = 206
	StatusMultiStatus          = 207
	StatusAlreadyReported      = 208
	StatusIMUsed               = 226

	StatusMultipleChoices   = 300
	StatusMovedPermanently  = 301
	StatusFound             = 302
	StatusSeeOther          = 303
	StatusNotModified       = 304
	StatusUseProxy          = 305
	_                       = 306
	StatusTemporaryRedirect = 307
	StatusPermanentRedirect = 308

	StatusBadRequest                   = 400
	StatusUnauthorized                 = 401
	StatusPaymentRequired              = 402
	StatusForbidden                    = 403
	StatusNotFound                     = 404
	StatusMethodNotAllowed             = 405
	StatusNotAcceptable                = 406
	StatusProxyAuthRequired            = 407
	StatusRequestTimeout               = 408
	StatusConflict                     = 409
	StatusGone                         = 410
	StatusLengthRequired               = 411
	StatusPreconditionFailed           = 412
	StatusRequestEntityTooLarge        = 413
	StatusRequestURITooLong            = 414
	StatusUnsupportedMediaType         = 415
	StatusRequestedRangeNotSatisfiable = 416
	StatusExpectationFailed            = 417
	StatusTeapot                       = 418
	StatusUnprocessableEntity          = 422
	StatusLocked                       = 423
	StatusFailedDependency             = 424
	StatusUpgradeRequired              = 426
	StatusPreconditionRequired         = 428
	StatusTooManyRequests              = 429
	StatusRequestHeaderFieldsTooLarge  = 431
	StatusUnavailableForLegalReasons   = 451

	StatusInternalServerError           = 500
	StatusNotImplemented                = 501
	StatusBadGateway                    = 502
	StatusServiceUnavailable            = 503
	StatusGatewayTimeout                = 504
	StatusHTTPVersionNotSupported       = 505
	StatusVariantAlsoNegotiates         = 506
	StatusInsufficientStorage           = 507
	StatusLoopDetected                  = 508
	StatusNotExtended                   = 510
	StatusNetworkAuthenticationRequired = 511
)

type Webfr interface {
	Start(address string) error
	Stop() error
	Get(path string, handlers ...handlerFunc) *Route
	Head(path string, handlers ...handlerFunc) *Route
	Post(path string, handlers ...handlerFunc) *Route
	Put(path string, handlers ...handlerFunc) *Route
	Patch(path string, handlers ...handlerFunc) *Route
	Delete(path string, handlers ...handlerFunc) *Route
	Connect(path string, handlers ...handlerFunc) *Route
	Options(path string, handlers ...handlerFunc) *Route
	Trace(path string, handlers ...handlerFunc) *Route
	Group(prefix string, routes []*Route) []*Route
	Static(prefix, root string)
	NotFound(handlers ...handlerFunc)
	Use(middlewares ...handlerFunc)
}

type webfr struct {
	httpServer       *fasthttp.Server
	router           *router
	registeredRoutes []*Route
	address          string
	middlewares      handlersChain
	settings         *Settings
}

type Settings struct {
	CaseInSensitive bool

	CacheSize              int
	HandleMethodNotAllowed bool

	HandleOPTIONS bool
	AutoRecover   bool

	ServerName string

	MaxRequestBodySize int

	MaxRouteParams int

	MaxRequestURLLength int

	Concurrency int

	Prefork bool

	DisableCaching bool

	DisableStartupMessage bool

	DisableKeepalive bool

	DisableDefaultDate bool

	DisableDefaultContentType bool

	DisableHeaderNormalizing bool

	ReadTimeout time.Duration

	WriteTimeout time.Duration

	IdleTimeout time.Duration

	TLSEnabled bool

	TLSCertPath string
	TLSKeyPath  string
}

type Route struct {
	Method   string
	Path     string
	Handlers handlersChain
}
