package main

import (
	"fmt"
	"log"
	"net/http"
	"photoend/internal/api"
)

const (
	PORT = 58058
)

func main() {
	router := api.Route()

	server := fmt.Sprintf(":%d", PORT)

	fmt.Printf("running on: %s\n", server)

	err := http.ListenAndServe(server, router)
	if err != nil {
		log.Fatalf("could not start server: %v", err)
	}
}
