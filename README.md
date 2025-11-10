### Photograph

## Build Instructions

> for both linux and windows
> I don't need to say that you need to install emscripten first, right?

```sh
emcmake cmake -G Ninja -B build-web
cmake --build build-web
```