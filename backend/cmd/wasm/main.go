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

	server := fmt.Sprintf("0.0.0.0:%d", PORT)

	fmt.Printf("Server running @%s\n", server)

	err := http.ListenAndServe(server, router)
	if err != nil {
		log.Fatalf("faialed to run server: %v", err)
	}
}
