package images

type Image struct {
	Topic  string `json:"topic"`
	RoomID string `json:"room_id"`
	// Url    int64  `json:"image_url"`
	// Width  int    `json:"width"`
	// Height int    `json:"height"`
	Data []any `json:"data"`
}
