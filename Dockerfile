FROM emscripten/emsdk:4.0.19 as wasm-builder

WORKDIR /workspace
RUN apt update && apt install -y ninja-build

COPY . /workspace
RUN emcmake cmake -G Ninja -B build-web && cmake --build build-web

# stage 2
FROM golang:1.24

WORKDIR /app

COPY backend /app/backend

COPY --from=wasm-builder /workspace/build-web/index.{html,wasm,js,data} /app/backend/internal/static/wasm/

WORKDIR /app/backend/cmd/wasm

RUN go build -o server

EXPOSE 58058

CMD ["./server"]