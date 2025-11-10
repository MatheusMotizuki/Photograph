FROM emscripten/emsdk:latest as wasm-builder

WORKDIR /workspace
# RUN apt-get update \
#     && apt-get install -y --no-install-recommends ninja-build git cmake \
#     && apt-get clean \
#     && rm -rf /var/lib/apt/lists/*

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