<div align="center">

# 🚢 Port-Ship-Scheduling-GA

### A genetic algorithm for port ship scheduling.

C++ GA for berth allocation, yard machines, yard storage and operation sequencing — with CPLEX comparison.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-17-00599C?logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![CPLEX](https://img.shields.io/badge/CPLEX-12.63-0080FF)](https://www.ibm.com/products/ilog-cplex-optimization-studio)

</div>

---

**Port-Ship-Scheduling-GA** is a C++ genetic algorithm that solves the port ship scheduling problem — integrating **berth allocation**, **yard machine assignment**, **yard storage** and **operation sequencing** — and compares the GA solution against a CPLEX exact baseline.

> [!NOTE]
> 中文项目：遗传算法集成的港口船舶调度系统，泊位 + 取料机 + 堆场 + 顺序优化，与 CPLEX 对比。

---

## Quickstart

```bash
git clone https://github.com/Windyhhh/Port-Ship-Scheduling-GA.git
cd Port-Ship-Scheduling-GA

# Compile with the provided scripts (paths pre-configured)
./compile_with_paths.bat

# With CPLEX 12.63
./compile_cplex1263.bat
./compile_and_run_cplex1263.bat
```

On WSL, run `install_cplex_wsl.sh` first.

---

## Features

- **Full problem scope** — berth + yard machine + storage + sequencing in one GA.
- **Exact baseline** — CPLEX comparison to validate solution quality.
- **Portable build** — ready-made compile scripts for Visual Studio and WSL.

---

## Project Structure

```
Port-Ship-Scheduling-GA/
├── genetic_algorithm.cpp        # GA implementation
├── compile_cplex1263.bat        # compile scripts
├── compile_and_run_cplex1263.bat
├── compile_with_paths.bat
├── install_cplex_wsl.sh
└── test.vcxproj
```

---

## License

MIT — free to use, modify and distribute.
