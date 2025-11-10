package filter

import (
	"encoding/json"
	"fmt"
	"photoend/internal/filter/create"
	"photoend/internal/filter/images"
	"photoend/internal/filter/join"
	"photoend/internal/filter/link"
	"photoend/internal/filter/link/delink"
	"photoend/internal/filter/mouse"
	"photoend/internal/filter/node"
	"photoend/internal/filter/node/denode"
	"photoend/internal/filter/node/selected"
)

type MessageFactory func() Filterable

var messageFactories = map[string]MessageFactory{
	"room.create":          func() Filterable { return &create.Create{} },
	"room.join":            func() Filterable { return &join.Join{} },
	"room.node.create":     func() Filterable { return &node.Node{} },
	"room.node.delete":     func() Filterable { return &denode.Denode{} },
	"room.node.select.pos": func() Filterable { return &selected.Selected{} },
	"room.link.create":     func() Filterable { return &link.Link{} },
	"room.link.delete":     func() Filterable { return &delink.Delink{} },
	"room.mouse.pos":       func() Filterable { return &mouse.Mouse{} },
	"room.image.share":     func() Filterable { return &images.Image{} },
}

func FilterMessage(raw []byte) (Filterable, error) {
	if len(raw) == 0 {
		return nil, fmt.Errorf("filter message, empty message received")
	}

	var Topic struct {
		Topic string `json:"topic"`
	}
	if err := json.Unmarshal(raw, &Topic); err != nil {
		return nil, fmt.Errorf("filter message, error unmarshalling topic: %w", err)
	}

	if Topic.Topic == "" {
		return nil, fmt.Errorf("filter message, topic is empty")
	}

	// Use the extracted topic
	factory, exists := messageFactories[Topic.Topic]
	if !exists {
		return nil, fmt.Errorf("filter message, unknown topic '%s'", Topic.Topic)
	}

	msg := factory()
	if err := json.Unmarshal(raw, msg); err != nil {
		return nil, fmt.Errorf("filter message, error unmarshalling message: %w", err)
	}

	if err := msg.ValidateContent(); err != nil {
		return nil, fmt.Errorf("filter message, content validation failed: %w", err)
	}

	return msg, nil
}
