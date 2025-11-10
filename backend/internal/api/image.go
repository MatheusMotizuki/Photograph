package api

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	manager "photoend/internal/websocket"
	"sync"
	"time"

	"github.com/gorilla/mux"
)

var (
	downloadCounts = make(map[string]int)
	downloadMutex  sync.Mutex
)

func ImageUpload(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}

	// limit upload size (10MB)
	r.Body = http.MaxBytesReader(w, r.Body, 10<<20)
	data, err := io.ReadAll(r.Body)
	if err != nil {
		http.Error(w, "failed to read body", http.StatusBadRequest)
		return
	}

	image_name := time.Now().UnixNano()

	// save uploaded bytes to a temporary file
	filename := fmt.Sprintf("./internal/storage/%d.jpg", image_name)
	if err := os.WriteFile(filename, data, 0644); err != nil {
		http.Error(w, "failed to save file", http.StatusInternalServerError)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	resp := map[string]string{"id": fmt.Sprintf("%d", image_name)}
	if err := json.NewEncoder(w).Encode(resp); err != nil {
		fmt.Printf("[%s] failed to encode response: %v\n", time.Now().Format(time.RFC3339), err)
	}
}

func ImageDownload(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	id := vars["id"]

	if id == "" {
		fmt.Printf("[%s] ImageDownload error: missing id (remote=%s)\n", time.Now().Format(time.RFC3339), r.RemoteAddr)
		http.Error(w, "missing id", http.StatusBadRequest)
		return
	}
	if len(id) != 19 {
		fmt.Printf("[%s] ImageDownload error: invalid id length (id=%s remote=%s)\n", time.Now().Format(time.RFC3339), id, r.RemoteAddr)
		http.Error(w, "missing id", http.StatusBadRequest)
		return
	}
	room := vars["room"]
	if room == "" {
		fmt.Printf("[%s] ImageDownload error: missing room (id=%s remote=%s)\n", time.Now().Format(time.RFC3339), id, r.RemoteAddr)
		http.Error(w, "missing room", http.StatusBadRequest)
		return
	}

	fmt.Printf("[%s] ImageDownload start: id=%s room=%s remote=%s\n", time.Now().Format(time.RFC3339), id, room, r.RemoteAddr)

	extensions := []string{".jpg", ".png", ".jpeg", ".gif"}
	var filename string
	for _, ext := range extensions {
		f := fmt.Sprintf("./internal/storage/%s%s", id, ext)
		if _, err := os.Stat(f); err == nil {
			filename = f
			break
		}
	}
	if filename == "" {
		fmt.Printf("[%s] ImageDownload not found: id=%s room=%s\n", time.Now().Format(time.RFC3339), id, room)
		http.Error(w, "file not found", http.StatusNotFound)
		return
	}

	fmt.Printf("[%s] ImageDownload serving file: %s (id=%s room=%s)\n", time.Now().Format(time.RFC3339), filename, id, room)

	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("[%s] ImageDownload failed to open file %s: %v\n", time.Now().Format(time.RFC3339), filename, err)
		http.Error(w, "failed to open file", http.StatusInternalServerError)
		return
	}
	defer file.Close()

	buf := make([]byte, 512)
	n, _ := file.Read(buf)
	contentType := http.DetectContentType(buf[:n])
	fmt.Printf("[%s] ImageDownload detected content-type=%s for file=%s\n", time.Now().Format(time.RFC3339), contentType, filename)

	file.Seek(0, io.SeekStart)

	w.Header().Set("Content-Type", contentType)
	w.Header().Set("Content-Disposition", "inline; filename="+id)
	written, err := io.Copy(w, file)
	if err != nil {
		fmt.Printf("[%s] ImageDownload failed to send file %s: %v\n", time.Now().Format(time.RFC3339), filename, err)
	} else {
		fmt.Printf("[%s] ImageDownload sent %d bytes for file %s (id=%s)\n", time.Now().Format(time.RFC3339), written, filename, id)
	}

	// Get number of users in the room
	users, err := manager.GetManager().GetUsersInRoom(room)
	if err != nil {
		fmt.Printf("[%s] ImageDownload could not get users in room %s: %v\n", time.Now().Format(time.RFC3339), room, err)
		http.Error(w, "could not get users in room", http.StatusInternalServerError)
		return
	}
	userCount := len(users)
	fmt.Printf("[%s] ImageDownload user count for room %s: %d\n", time.Now().Format(time.RFC3339), room, userCount)

	// Track downloads and remove file after all users have downloaded
	downloadMutex.Lock()
	downloadCounts[filename]++
	currentCount := downloadCounts[filename]
	fmt.Printf("[%s] ImageDownload incremented download count for %s: %d/%d\n", time.Now().Format(time.RFC3339), filename, currentCount, userCount)
	if downloadCounts[filename] >= userCount-1 {
		if err := os.Remove(filename); err != nil {
			fmt.Printf("[%s] ImageDownload failed to remove file %s: %v\n", time.Now().Format(time.RFC3339), filename, err)
		} else {
			fmt.Printf("[%s] ImageDownload removed file %s after %d downloads\n", time.Now().Format(time.RFC3339), filename, currentCount)
		}
		delete(downloadCounts, filename)
	}
	downloadMutex.Unlock()
}
