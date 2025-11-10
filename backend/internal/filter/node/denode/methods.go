package denode

import (
	"fmt"
	"photoend/internal/interfaces"
)

func (d Denode) GetTopic() string {
	return d.Topic
}

func (d Denode) ValidateContent() error {
	if d.RoomID == "" {
		return fmt.Errorf("selected, room id cannot be empty")
	}
	return nil
}

func (d Denode) Handle(manager interfaces.RoomManager, conn any) error {
	var senderID string
	if userConn, ok := conn.(interfaces.UserRelated); ok {
		senderID = userConn.GetUserId()
	}

	if err := manager.BroadcastMessage(d.RoomID, d.Topic, d.Data, senderID); err != nil {
		return fmt.Errorf("could not broadcast message: %v", err)
	}
	return nil
}
