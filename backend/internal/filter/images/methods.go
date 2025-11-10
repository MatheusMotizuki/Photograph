package images

import (
	"fmt"
	"photoend/internal/interfaces"
)

func (i Image) GetTopic() string {
	return i.Topic
}

func (i Image) ValidateContent() error {
	if i.RoomID == "" {
		return fmt.Errorf("mouse, room id cannot be empty")
	}
	return nil
}

func (i Image) Handle(manager interfaces.RoomManager, conn any) error {
	var senderID string
	if userConn, ok := conn.(interfaces.UserRelated); ok {
		senderID = userConn.GetUserId()
	}

	if err := manager.BroadcastMessage(i.RoomID, i.Topic, i.Data, senderID); err != nil {
		return fmt.Errorf("could not broadcast message: %v", err)
	}
	return nil
}
