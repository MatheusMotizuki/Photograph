package join

type Join struct {
	// message topic
	Topic string `json:"topic"`
	// rom unique identifier
	RoomID string `json:"room_id"`
}
