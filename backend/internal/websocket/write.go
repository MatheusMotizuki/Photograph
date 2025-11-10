package manager

import "github.com/gorilla/websocket"

func (c *Connection) Write() {
	for {
		message, ok := <-c.egress
		if !ok {
			// Channel closed, exit the read loop
			return
		}

		if err := c.ws.WriteMessage(websocket.BinaryMessage, message); err != nil {
			// Error writing to websocket
			return
		}
	}
}
