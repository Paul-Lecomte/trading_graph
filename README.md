           #  Trading Graph

![Build Status](https://img.shields.io/github/actions/workflow/status/Paul-Lecomte/trading_graph/ci.yml?branch=main)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Powered by Lightweight Charts](https://img.shields.io/badge/Powered%20by-Lightweight%20Charts-4A90E2)](https://github.com/tradingview/lightweight-charts)

A modern TradingView clone built with [TradingView's Lightweight Charts library](https://github.com/tradingview/lightweight-charts).

![Trading Graph Banner](docs/screenshot.png)

---

##  Features

- Interactive financial charts
- Customizable chart options
- Real-time data updates _(planned)_

---

## Installation

### Clone the repository

```sh
git clone https://github.com/Paul-Lecomte/trading_graph.git
cd trading_graph
```

### Install dependencies

```sh
npm install
```

### Run the application

```sh
npm start
```

---

##  Preview

<!-- Add a screenshot of your app below -->
![Trading Graph Screenshot](docs/screenshot.png)

---

##  Contributing

Contributions are welcome! Please open an issue or submit a pull request.

---

##  License

This project is for educational purposes and uses the open-source Lightweight Charts library.

---

> _Made with ❤️ for learning and experimentation._

## Advanced C++ Example

This repository primarily hosts a Nuxt/Vue application, but it also includes an advanced modern C++20 example demonstrating concepts, CRTP, strong types, constexpr algorithms, fold expressions, ranges, and lightweight type erasure.

File location:
- examples\advanced_cpp_examples.cpp

Build and run on Windows (PowerShell):

1. Ensure you have a C++20-capable compiler installed (MinGW g++ or LLVM clang++).
2. From the project root, create an output directory if it doesn't exist:
   ```powershell
   if (!(Test-Path .\bin)) { New-Item -ItemType Directory -Path .\bin | Out-Null }
   ```
3. Compile with g++ (MinGW):
   ```powershell
   g++ -std=c++20 -O2 -Wall -Wextra -pedantic examples\advanced_cpp_examples.cpp -o bin\advanced_cpp_examples.exe
   ```
   Or compile with clang++:
   ```powershell
   clang++ -std=c++20 -O2 -Wall -Wextra -pedantic examples\advanced_cpp_examples.cpp -o bin\advanced_cpp_examples.exe
   ```
4. Run:
   ```powershell
   .\bin\advanced_cpp_examples.exe
   ```

The example outputs results for unit-strong types, a variadic fold-sum, CRTP-based printable objects, a type-erased drawable scene, and uses ranges to generate prime numbers.
