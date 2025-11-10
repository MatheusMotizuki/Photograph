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

	fmt.Printf("Server running @%d\n", PORT)

	err := http.ListenAndServe(server, router)
	if err != nil {
		log.Fatalf("faialed to run server: %v", err)
	}
}
