# SimpleDLLInjector

Inject custom DLLs into Windows processes with a minimal, Qt-based GUI. The application lists all running processes and their target architecture (x64 or x86) and allows you to inject .dll from one GUI.

---

## Features

- Supports **x64** and **x86** target applications.
- Very simple **Qt GUI frontend**.
- Headless support for using without Qt.
- Built with **C++17** and **CMake**.
- Automatically deploys with `windeployqt` for easy distribution.

---

## Build Instructions

### Prerequisites
- CMake **≥ 3.5**
- Qt **6.x** (with `Widgets` module)
- C++17 compatible compiler (e.g., MSVC, Clang)
- Windows OS

---

### Build Steps

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/SimpleDLLInjector.git
   cd SimpleDLLInjector
   ```
Configure and build:
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build . --config Release
   ```
Configure and build a headless x86 injector under \backend\app to inject into x86 applications and place it into the same folder as GUI executable. You can skip this step if you don't aim for x86 apps.

Output: The final executable and necessary Qt dependencies will be placed in the bin/ directory.

---

## Disclaimer  

This project is for educational purposes only. Use it responsibly and only on software you own or have permission to interact with. Unauthorized DLL injection may violate terms of service or laws.
