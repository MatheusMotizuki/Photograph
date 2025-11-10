package node

type Node struct {
	Topic  string `json:"topic"`
	RoomID string `json:"room_id"`
	Data   []any  `json:"data"`
}
