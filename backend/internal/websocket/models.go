package manager

import (
	"sync"

	"github.com/gorilla/websocket"
)

type Connection struct {
	ws          *websocket.Conn
	egress      chan []byte
	ingress     chan []byte
	subprotocol string
	roomID      string
	creator     bool
	UserId      string
}

type Manager struct {
	connections map[*Connection]bool
	rooms       map[string]map[*Connection]bool
	sync.RWMutex
}

func (c *Connection) GetUserId() string {
	return c.UserId
}
