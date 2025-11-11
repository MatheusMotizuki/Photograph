package api

import (
	"net/http"

	"github.com/gorilla/mux"
)

func Route() *mux.Router {
	r := mux.NewRouter()

	// Serve root index.html
	r.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		http.ServeFile(w, r, "/app/backend/internal/static/index.html")
	}).Methods("GET")

	// Serve /app/ index.html (SPA entry point)
	r.HandleFunc("/app/", func(w http.ResponseWriter, r *http.Request) {
		http.ServeFile(w, r, "/app/backend/internal/static/wasm/index.html")
	}).Methods("GET")

	// Serve static files under /app/
	r.PathPrefix("/app/").Handler(
		http.StripPrefix("/app/", http.FileServer(http.Dir("/app/backend/internal/static/wasm/"))),
	)

	return r
}
