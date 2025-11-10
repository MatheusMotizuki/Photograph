package delink

type Delink struct {
	Topic  string    `json:"topic"`
	RoomID string    `json:"room_id"`
	Data   []float64 `json:"data"`
}
