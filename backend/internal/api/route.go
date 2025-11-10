package api

import (
	manager "photoend/internal/websocket"

	"github.com/gorilla/mux"
)

func Route() *mux.Router {
	r := mux.NewRouter()

	r.HandleFunc("/ws", manager.GetManager().HandleWS)
	r.HandleFunc("/upload", ImageUpload)
	r.HandleFunc("/download/{id}/{room}", ImageDownload)
	return r
}
