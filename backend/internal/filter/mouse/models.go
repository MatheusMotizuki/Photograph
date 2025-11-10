package mouse

type Mouse struct {
	Topic  string    `json:"topic"`
	RoomID string    `json:"room_id"`
	Data   []float64 `json:"data"`
}
