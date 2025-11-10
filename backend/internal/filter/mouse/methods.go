package mouse

import (
	"fmt"
	"photoend/internal/interfaces"
)

func (m Mouse) GetTopic() string {
	return m.Topic
}

func (m Mouse) ValidateContent() error {
	if m.RoomID == "" {
		return fmt.Errorf("mouse, room id cannot be empty")
	}
	return nil
}

func (m Mouse) Handle(manager interfaces.RoomManager, conn any) error {
	var senderID string
	if userConn, ok := conn.(interfaces.UserRelated); ok {
		senderID = userConn.GetUserId()
	}

	if err := manager.BroadcastMessage(m.RoomID, m.Topic, m.Data, senderID); err != nil {
		return fmt.Errorf("could not broadcast message: %v", err)
	}
	return nil
}
