package manager

import (
	"log"
	"photoend/internal/filter"
)

func (c *Connection) Read() {
	manager := GetManager()

	for {
		raw, ok := <-c.ingress
		if !ok {
			return
		}

		msg, err := filter.FilterMessage(raw)
		if err != nil {
			log.Printf("message filtering failed: %v\n", err)
			continue
		}

		go func(message filter.Filterable) {
			if err := message.Handle(manager, c); err != nil {
				log.Printf("error handling %s message: %v\n", message.GetTopic(), err)
			}
		}(msg)
	}
}
