package join

import (
	"fmt"
	"photoend/internal/interfaces"
)

func (j Join) GetTopic() string {
	return j.Topic
}

func (j Join) ValidateContent() error {
	if j.RoomID == "" {
		return fmt.Errorf("join, room id cannot be empty")
	}
	return nil
}

func (j Join) Handle(manager interfaces.RoomManager, conn any) error {
	if err := manager.AddConnectionToRoom(j.RoomID, conn); err != nil {
		return fmt.Errorf("failed to add connection to room: %w", err)
	}

	fmt.Printf("connection: %v joined room: %s\n", conn, j.RoomID)

	return nil
}
