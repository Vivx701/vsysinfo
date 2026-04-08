# 🧠 VSysInfo

> Modern cross-platform C++ system information library (Windows, Linux)

---

## 🚀 Overview

**VSysInfo** is a modern, high-performance C++ library for retrieving real-time system information across multiple platforms, including **Windows** and **Linux**.

Designed with a focus on scalability, modularity, and efficiency, it is well-suited for system monitoring tools, dashboards, and performance-critical applications.

---

## ✨ Features

- Cross-platform support (Windows & Linux)
- High-performance system data access
- Thread-safe and modular architecture
- Clean and extensible API design
- CMake-based build system
- Multiple Windows toolchain support:
  - MinGW
  - MSVC (Ninja)
  - MSVC (Visual Studio)

---

## 🛠️ Build Instructions

### Linux

```bash
chmod +x build.sh
./build.sh
```

---

### Windows

#### MinGW
```powershell
.\build.ps1 mingw
```

#### MSVC (Ninja)
```powershell
.\build.ps1 msvc
```

#### MSVC (Visual Studio)
```powershell
.\build.ps1 msvc-vs
```

---

## ⚠️ Configuration Note

Before running the build scripts, ensure that you review and update any environment-specific variables such as:

- Compiler/toolchain paths  
- Build configuration options  
- System-specific dependencies  

---

## 🧪 Testing

```bash
.\vsys_tests.exe
```

---

## 🔧 Example Usage

```cpp
#include <VSysInfo/SystemInfo.h>

int main() {
    auto cpu = VSysInfo::CPU::usage();
    auto mem = VSysInfo::Memory::usage();

    std::cout << "CPU Usage: " << cpu << "%\n";
    std::cout << "Memory Usage: " << mem << "%\n";

    return 0;
}
```

---

## 📜 License

This project is licensed under the **MIT License**.  
See the LICENSE file for details.

---

## 🤝 Contributing

Contributions are welcome. Please open an issue or submit a pull request for any improvements or fixes.
