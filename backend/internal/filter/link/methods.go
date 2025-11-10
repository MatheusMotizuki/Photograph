package link

import (
	"fmt"
	"photoend/internal/interfaces"
)

func (l Link) GetTopic() string {
	return l.Topic
}

func (l Link) ValidateContent() error {
	if l.RoomID == "" {
		return fmt.Errorf("mouse, room id cannot be empty")
	}
	return nil
}

func (l Link) Handle(manager interfaces.RoomManager, conn any) error {
	var senderID string
	if userConn, ok := conn.(interfaces.UserRelated); ok {
		senderID = userConn.GetUserId()
	}

	if err := manager.BroadcastMessage(l.RoomID, l.Topic, l.Data, senderID); err != nil {
		return fmt.Errorf("could not broadcast message: %v", err)
	}
	return nil
}
