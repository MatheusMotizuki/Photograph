package create

import (
	"fmt"
	"photoend/internal/interfaces"
)

func (c Create) GetTopic() string {
	return c.Topic
}

func (c Create) ValidateContent() error {
	if c.RoomID == "" {
		return fmt.Errorf("create, room id cannot be empty")
	}
	return nil
}

func (c Create) Handle(manager interfaces.RoomManager, conn any) error {
	if err := manager.CreateRoom(c.RoomID, conn); err != nil {
		return fmt.Errorf("failed to create room: %w", err)
	}

	if err := manager.AddConnectionToRoom(c.RoomID, conn); err != nil {
		return fmt.Errorf("failed to add connection to room: %w", err)
	}

	return nil
}
