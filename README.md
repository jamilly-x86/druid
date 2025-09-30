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

## Compiling from source

This project uses CMake with presets for easy configuration and building across different platforms and architectures.

To see all available presets for your platform:
```bash
cmake --list-presets
```

### Prerequisites

- CMake 3.31+, Ninja 1.10+
- g++15+, Clang-20+, MSVC 17.14+
- Git

### Windows

For supported presets review [platform-windows.json](./cmake/preset/platform-windows.json)

#### Using Visual Studio 2022

1. Clone the repository:
   ```powershell
   git clone https://github.com/druidengine/druid
   cd druid
   ```

1. Configure the project using a preset:
   ```powershell
   cmake --preset x64-windows-msvc-debug
   ```

1. Build the project:
   ```powershell
   cmake --build --preset x64-windows-msvc-debug
   ```

1. Run the Pong example:
   ```powershell
   .\build\app\pong\druid-pong.exe
   ```

1. Run unit tests:
   ```powershell
   ctest --preset x64-windows-msvc-debug
   ```

### Linux

For supported presets review [platform-linux.json](./cmake/preset/platform-linux.json)

#### Using GCC

1. Clone the repository:
   ```bash
   git clone https://github.com/druidengine/druid
   cd druid
   ```

1. Install dependencies (Ubuntu/Debian):
   ```bash
   sudo apt install -y curl tar git zip unzip make autoconf libtool python3-jinja2 ninja-build cmake
   sudo apt install -y '^libxcb.*-dev' libfontconfig1-dev libfreetype6-dev libx11-dev libxext-dev libxfixes-dev libgl1-mesa-dev libglu1-mesa-dev libegl1-mesa-dev
   sudo apt install -y libxrender-dev libxi-dev libxcb1-dev libxcb-glx0-dev libxcb-keysyms1-dev libxcb-image0-dev libxcb-shm0-dev libx11-xcb-dev libxkbcommon-dev
   sudo apt install -y libxkbcommon-x11-dev libxcb-icccm4-dev libxcb-sync0-dev libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev libxcb-render-util0-dev libxinerama-dev
   ```

1. Configure the project:
   ```bash
   cmake --preset x64-linux-gcc-debug
   ```

1. Build the project:
   ```bash
   cmake --build --preset x64-linux-gcc-debug
   ```

1. Run the Pong example:
   ```bash
   ./build/app/pong/druid-pong
   ```

1. Run unit tests:
   ```bash
   ctest --preset x64-linux-gcc-debug
   ```

## Community and contributing

To get started contributing to the project, review the [CONTRIBUTING.md](CONTRIBUTING.md) document.
