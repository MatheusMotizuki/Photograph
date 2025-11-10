package interfaces

type RoomManager interface {
	CreateRoom(roomID string, conn any) error
	AddConnectionToRoom(roomID string, conn any) error
	BroadcastMessage(roomID string, topic string, message any, senderID string) error
}

type UserRelated interface {
	GetUserId() string
}
