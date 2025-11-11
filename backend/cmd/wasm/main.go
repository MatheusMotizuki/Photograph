package main

import (
	"backend/internal/api"
	"fmt"
	"log"
	"net/http"
)

const (
	PORT = 58058
)

func main() {
	// create router
	router := api.Route()

	server := fmt.Sprintf(":%d", PORT)

	err := http.ListenAndServe(server, router)
	if err != nil {
		log.Fatalf("failed to run server: %v", err)
	}
}
