### Photograph

## Build Instructions

### Prerequisites
- CMake 3.20+
- C++17 compiler
- vcpkg (for OpenCV, libhv, and cpr)

### Windows

1. **Install vcpkg**
   ```powershell
   cd C:\
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install
   ```

2. **Install dependencies**
   ```powershell
   .\vcpkg install opencv4:x64-windows-static libhv:x64-windows-static cpr:x64-windows-static
   ```

3. **Build**
   ```powershell
   cmake -B build -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static
   cmake --build build --config Release
   ```

### Linux

> vcpkg maybe not the best option for linux, but it is what it is, maybe in the future i'll remake this.

1. **Install vcpkg**
   ```bash
   cd ~
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   ./bootstrap-vcpkg.sh
   ./vcpkg integrate install
   ```

2. **Install dependencies**
   ```bash
   ./vcpkg install opencv4:x64-linux libhv:x64-linux cpr:x64-linux
   ```

3. **Build**
   ```bash
   cmake -B build -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-linux
   cmake --build build --config Release
   ```

### Output
- **Windows**: `build\bin\Release\photoGraph.exe`
- **Linux**: `build/bin/photoGraph`

**Note**: The executable is fully static and requires no external DLLs or libraries.