# Druid

<p align="center">
    <img src="logo.png" alt="Druid Engine Logo" width="384" height="256">
</p>

## 2D cross-platform game engine

[Druid](https://druidengine.org) is a 2D game engine written in modern C++ to explore contemporary C++ features and graphics programming techniques. Built with performance and educational value in mind, it demonstrates the use of C++20 modules, cross-platform development, and an opportunity to learn.

## Features

- **Modern C++**: Utilizes C++20 modules and contemporary C++ patterns
- **Cross-Platform**: Supports Windows, Linux, macOS, Android, and WebAssembly
- **Educational Focus**: Designed as a learning platform for modern C++ and graphics programming

## Project Structure

- `src/core/` - Core utilities and fundamental concepts
- `src/graphics/` - Graphics rendering system with Vulkan backend
- `app/pong/` - Example Pong game implementation
- `cmake/` - CMake configuration and preset definitions
- `vcpkg/` - Package manager integration for dependencies

## Building

This project uses CMake with presets for easy configuration and building across different platforms and architectures.

### Prerequisites

- CMake 3.25 or later
- A C++20 compliant compiler (MSVC 2022, GCC 12+, or Clang 15+)
- Git

### Windows

#### Using Visual Studio 2022

1. Clone the repository:
   ```powershell
   git clone https://github.com/druidengine/druid
   cd druid
   ```

2. Configure the project using a preset:
   ```powershell
   cmake --preset x64-windows-msvc-debug
   ```

3. Build the project:
   ```powershell
   cmake --build --preset x64-windows-msvc-debug
   ```

4. Run the Pong example:
   ```powershell
   .\build\app\pong\druid-pong.exe
   ```

#### Alternative Presets for Windows

- **Release build**: `x64-windows-msvc-release`
- **ARM64 architecture**: `arm64-windows-msvc-debug`

### Linux

#### Using GCC

1. Clone the repository:
   ```bash
   git clone https://github.com/druidengine/druid
   cd druid
   ```

2. Install dependencies (Ubuntu/Debian):
   ```bash
   sudo apt update
   sudo apt install build-essential cmake ninja-build pkg-config
   ```

3. Configure the project:
   ```bash
   cmake --preset x64-linux-gcc-debug
   ```

4. Build the project:
   ```bash
   cmake --build --preset x64-linux-gcc-debug
   ```

5. Run the Pong example:
   ```bash
   ./build/app/pong/druid-pong
   ```

#### Alternative Presets for Linux

- **Release build**: `x64-linux-gcc-release`
- **Clang compiler**: `x64-linux-clang-debug`
- **ARM64 architecture**: `arm64-linux-gcc-debug`

### Available CMake Presets

The project includes comprehensive preset configurations for:

- **Configurations**: Debug, Release, RelWithDebInfo
- **Platforms**: Windows (x64, ARM64), Linux (x64, ARM64), macOS (ARM64), Android, WebAssembly
- **Compilers**: MSVC, GCC, Clang, Emscripten, Android NDK
- **Build Systems**: Ninja, Visual Studio

To see all available presets:
```bash
cmake --list-presets
```

## Development

### Running Tests

```bash
# After building, run the test suite
ctest --preset <your-preset-name>
```

### Project Configuration

The project uses vcpkg for dependency management and CMake presets for configuration management. All presets automatically handle dependency installation and configuration.

## Contributing

This is primarily an educational project for exploring modern C++ and graphics programming concepts. Contributions and suggestions are welcome.

## License

See `LICENSE` file for license information.
