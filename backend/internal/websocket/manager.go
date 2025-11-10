package manager

import (
	"encoding/json"
	"fmt"
	"net/http"
	"sync"
	"time"

	"github.com/goombaio/namegenerator"
	"github.com/gorilla/websocket"
)

var (
	upgrader = websocket.Upgrader{
		CheckOrigin: func(r *http.Request) bool {
			return true
		},
		ReadBufferSize:  1024,
		WriteBufferSize: 1024,
	}

	manager *Manager
	once    sync.Once
)

func GetManager() *Manager {
	once.Do(func() {
		manager = &Manager{
			connections: make(map[*Connection]bool),
			rooms:       make(map[string]map[*Connection]bool),
		}
	})
	return manager
}

func (m *Manager) HandleWS(w http.ResponseWriter, r *http.Request) {
	ws, err := upgrader.Upgrade(w, r, nil)
	if err != nil {
		return
	}

	conn := &Connection{
		ws:          ws,
		egress:      make(chan []byte),
		ingress:     make(chan []byte),
		subprotocol: r.Header.Get("Sec-WebSocket-Protocol"),
		roomID:      "",
		UserId:      GenerateUniqueToken(),
	}

	go conn.Read()
	go conn.Write()

	m.AddConnection(conn)
	defer m.RemoveConnection(conn)

	for {
		_, message, err := ws.ReadMessage()
		if err != nil {
			if websocket.IsCloseError(err, websocket.CloseNormalClosure, websocket.CloseGoingAway, websocket.CloseAbnormalClosure) {
				fmt.Println("WebSocket connection closed normal")
			}
			return
		}
		conn.ingress <- message
	}
}

func (m *Manager) AddConnection(conn *Connection) {
	m.Lock()
	defer m.Unlock()

	m.connections[conn] = true
}

func (m *Manager) RemoveConnection(conn *Connection) {
	m.Lock()
	defer m.Unlock()

	if _, ok := m.connections[conn]; ok {
		delete(m.connections, conn)
		conn.ws.Close()
		close(conn.ingress)

		// Remove connection from room if present
		if conn.roomID != "" {
			if room, exists := m.rooms[conn.roomID]; exists {
				delete(room, conn)

				if len(room) == 0 { // Delete room if empty
					delete(m.rooms, conn.roomID)
				}
			}
			conn.roomID = ""
		}

		go func() {
			close(conn.egress)
			for range conn.egress {
			}
		}()
		conn.ws.Close()
	}
}

func (m *Manager) CreateRoom(roomID string, conn any) error {
	m.Lock()
	defer m.Unlock()

	if _, exists := m.rooms[roomID]; exists {
		return fmt.Errorf("room %s already exists", roomID)
	}

	m.rooms[roomID] = make(map[*Connection]bool)
	if conn != nil {
		conn.(*Connection).creator = true
	}
	return nil
}

func (m *Manager) AddConnectionToRoom(roomID string, conn any) error {
	m.Lock()
	defer m.Unlock()

	connection, ok := conn.(*Connection)
	if !ok {
		return fmt.Errorf("invalid connection type")
	}

	if _, exists := m.rooms[roomID]; !exists {
		return fmt.Errorf("room %s does not exist", roomID)
	}

	m.rooms[roomID][connection] = true
	conn.(*Connection).roomID = roomID
	return nil
}

func (m *Manager) ListRooms() map[string]map[*Connection]bool {
	return m.rooms
}

func (m *Manager) BroadcastMessage(roomID string, topic string, message any, senderID string) error {
	allUsers, err := m.GetUsersInRoom(roomID)
	if err != nil {
		return err
	}

	// Create a wrapper to include sender info
	wrappedMsg := map[string]any{
		"topic":  topic,
		"sender": senderID,
		"data":   message,
	}

	data, err := json.Marshal(wrappedMsg)
	if err != nil {
		return err
	}

	for _, user := range allUsers {
		if user.UserId != senderID {
			user.egress <- data
		}
	}

	return nil
}

func (m *Manager) GetUsersInRoom(roomID string) ([]*Connection, error) {
	m.Lock()
	defer m.Unlock()

	room, exists := m.rooms[roomID]
	if !exists {
		return nil, fmt.Errorf("room %s does not exist", roomID)
	}

	users := make([]*Connection, 0, len(room))
	for conn := range room {
		users = append(users, conn)
	}
	return users, nil
}

func GenerateUniqueToken() string {
	seed := time.Now().UTC().UnixNano()
	nameGen := namegenerator.NewNameGenerator(seed)

	name := nameGen.Generate()

	return name
}
