package filter

import "photoend/internal/interfaces"

type Filterable interface {
	GetTopic() string
	ValidateContent() error
	Handle(manager interfaces.RoomManager, conn interface{}) error
}
