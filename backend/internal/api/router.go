package api

import (
	"net/http"

	"github.com/gorilla/mux"
)

func Route() *mux.Router {
	r := mux.NewRouter()

	r.HandleFunc("/",
		func(w http.ResponseWriter, r *http.Request) {
			http.ServeFile(w, r, "/app/backend/internal/static/index.html")
		}).Methods("GET")

	r.HandleFunc("/app/", func(w http.ResponseWriter, r *http.Request) {
		http.ServeFile(w, r, "/app/backend/internal/static/wasm/index.html")
	}).Methods("GET")

	return r
}
