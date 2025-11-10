package delink

import (
	"fmt"
	"photoend/internal/interfaces"
)

func (d Delink) GetTopic() string {
	return d.Topic
}

func (d Delink) ValidateContent() error {
	if d.RoomID == "" {
		return fmt.Errorf("mouse, room id cannot be empty")
	}
	return nil
}

func (d Delink) Handle(manager interfaces.RoomManager, conn any) error {
	var senderID string
	if userConn, ok := conn.(interfaces.UserRelated); ok {
		senderID = userConn.GetUserId()
	}

	if err := manager.BroadcastMessage(d.RoomID, d.Topic, d.Data, senderID); err != nil {
		return fmt.Errorf("could not broadcast message: %v", err)
	}
	return nil
}
