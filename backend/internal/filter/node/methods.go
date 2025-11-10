package node

import (
	"fmt"
	"photoend/internal/interfaces"
)

func (n Node) GetTopic() string {
	return n.Topic
}

func (n Node) ValidateContent() error {
	return nil
}

func (n Node) Handle(manager interfaces.RoomManager, conn any) error {
	var senderID string
	if userConn, ok := conn.(interfaces.UserRelated); ok {
		senderID = userConn.GetUserId()
	}

	if err := manager.BroadcastMessage(n.RoomID, n.Topic, n.Data, senderID); err != nil {
		return fmt.Errorf("could not broadcast message: %v", err)
	}
	return nil
}
