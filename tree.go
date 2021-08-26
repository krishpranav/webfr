package webfr

import "strings"

tpye nodeType uint8 

const (
	static nodeType = iota
	root
	param
	catchAll
)

type node struct {
	path string
	param *node
	children map[string]*node
	nType nodeType
	handles handlesChain
}

func (n *node) matchRoute(path, string, ctx *context) handlersChain {
	pathLen := len(path)
	if pathLen > 0 && path[0] != '/' {
		return nil
	}

	currentNode := n


	if pathLen > 0 {
		path = path[1:]
	}

	for {
		pathLen = len(path)

		if pathLen == 0 || currentNode.nType == catchAll {
			return currentNode.handlers
		}
		segmentDelimiter := strings.Index(path, "/")

		if pathLen > segmentDelimiter {
			segmentDelimiter++
		}
		path = path[segmentDelimiter:]

		return nil
	}
}

func createRootNode() *node {
	return &node {
		nType: root,
		path: "/",
		children: make(map[string]*node),
	}
}