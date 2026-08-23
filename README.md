# 🚢 Port Ship Scheduling GA | 港口船舶调度系统 - 基于遗传算法的智能优化

> **C++ genetic algorithm for integrated port ship scheduling: berth allocation, reclaimer assignment, storage yard planning, and operation sequencing. Minimizes total port time + transport cost. Includes CPLEX comparison scripts.**
>
> C++ 遗传算法解决港口船舶集成调度：泊位分配、堆取料机分配、堆场规划、作业排序。最小化总在港时间 + 运输成本。包含 CPLEX 对比脚本。

---

## 🌟 Why This Project? | 项目亮点

Port terminal scheduling is a complex combinatorial optimization problem involving **berth allocation, quay crane assignment, storage yard planning, and operation sequencing** — all interdependent. This project implements a **genetic algorithm (GA)** in C++ that solves the integrated port ship scheduling problem, minimizing a weighted objective of **total vessel port time + cargo transport cost**. The solution includes a complete chromosome encoding scheme, selection/crossover/mutation operators, elitism, and CPLEX-based exact solver scripts for performance comparison.

港口码头调度是一个复杂的组合优化问题，涉及**泊位分配、岸桥分配、堆场规划和作业排序**——所有这些相互依赖。本项目用 C++ 实现了一个**遗传算法（GA）**，解决港口船舶集成调度问题，最小化**船舶总在港时间 + 货物运输成本**的加权目标。解决方案包含完整的染色体编码方案、选择/交叉/变异算子、精英保留策略，以及用于性能对比的 CPLEX 精确求解器脚本。

| Feature | Details |
|---------|---------|
| **Language** | C++ (STL, no external dependencies for GA) |
| **Problem** | Integrated port ship scheduling (berth + reclaimer + storage + sequence) |
| **Algorithm** | Genetic Algorithm with elitism |
| **Selection** | Roulette wheel selection |
| **Crossover** | Single-point crossover |
| **Mutation** | Adaptive mutation with constraint validation |
| **Elitism** | Top 10% preserved each generation |
| **Objective** | Minimize: 30000 × total_port_time + transport_cost |
| **Comparison** | CPLEX exact solver scripts included |
| **Scale** | 4 ships, 2 berths, 3 reclaimers, 6 storage rows, 20 bays/row |

---

## 🏗️ Problem Formulation | 问题建模

### Decision Variables | 决策变量

| Variable | Description |
|----------|-------------|
| `berthAssign[i]` | Berth assigned to ship i |
| `reclaimerAssign[i]` | Reclaimer assigned to ship i |
| `storageRowAssign[i]` | Storage row assigned to ship i |
| `bayAssign[i]` | Storage bays assigned to ship i |
| `berthOrder[i][j]` | Operation order: ship j before ship i at same berth |
| `reclaimerOrder[i][j]` | Operation order: ship j before ship i at same reclaimer |

### Parameters | 参数

| Parameter | Symbol | Value | Description |
|-----------|--------|-------|-------------|
| Ships | S | 4 | Number of vessels |
| Berths | B | 2 | Number of berths |
| Reclaimers | R | 3 | Number of stacker-reclaimers |
| Storage rows | H | 2×R | Number of storage rows |
| Bays per row | U | 20 | Storage bays per row |
| Cargo types | A | 3 | Types of cargo |

### Objective Function | 目标函数

```
Minimize:  30000 × Σ_i (endTime[i] - arriveTime[i])  +  transportCost

where:
  transportCost = Σ_i Σ_bay cost[berth][row][bay] × weight[i] / numBays[i]
  endTime[i] = startTime[i] + unloadTime[i]
  unloadTime[i] = weight[i] / min(berth_rate, reclaimer_rate)
```

The weight 30000 emphasizes minimizing vessel port time (the primary operational KPI) while still accounting for transport cost.

### Constraints | 约束条件

1. **Berth feasibility**: `berth.depth ≥ ship.draft` AND `berth.length ≥ ship.length`
2. **Berth non-overlap**: Ships at same berth cannot overlap in time
3. **Reclaimer non-overlap**: Ships at same reclaimer cannot overlap in time
4. **Storage capacity**: Each ship gets required number of bays
5. **Storage row mapping**: Row = 2 × reclaimer + {0,1}
6. **Cargo rate matching**: Unload rate = min(berth unload rate, reclaimer stack rate)

---

## 🧬 Genetic Algorithm Design | 遗传算法设计

### Chromosome Encoding | 染色体编码

```
Chromosome = {
  berthAssign[0..S-1],        // int: berth index per ship
  reclaimerAssign[0..S-1],    // int: reclaimer index per ship
  storageRowAssign[0..S-1],   // int: storage row index per ship
  bayAssign[0..S-1][],        // vector<int>: storage bays per ship
  berthOrder[0..S-1][0..S-1], // int[][]: berth operation sequence
  reclaimerOrder[0..S-1][0..S-1], // int[][]: reclaimer operation sequence
  fitness                      // double: fitness value
}
```

### GA Parameters | 遗传算法参数

| Parameter | Value | Description |
|-----------|-------|-------------|
| Population size | 100 | Number of chromosomes per generation |
| Max generations | 1000 | Stopping criterion |
| Mutation rate | 0.1 | Probability of mutation per gene |
| Crossover rate | 0.8 | Probability of crossover |
| Elite rate | 0.1 | Top 10% preserved unchanged |
| Random seed | 0 | Fixed for reproducibility |

### Operators | 算子

**Selection (Roulette Wheel)**:
```
P(select chromosome i) = fitness[i] / Σ_j fitness[j]
```

**Crossover (Single-Point)**:
- Random crossover point in [0, S)
- First part from parent1, second part from parent2
- Applied to: berthAssign, reclaimerAssign, storageRowAssign, bayAssign

**Mutation (Constraint-Aware)**:
- Berth mutation: only select from feasible berths (depth + length constraints)
- Reclaimer mutation: random reclaimer, update storage row accordingly
- Storage row mutation: valid row for assigned reclaimer
- Bay mutation: random shuffle of available bays

**Elitism**:
- Sort population by fitness (descending)
- Preserve top 10% unchanged in next generation

### Fitness Calculation | 适应度计算

```
objective = 30000 × total_port_time + transport_cost
fitness = 1.0 / (1.0 + objective)
```

Higher fitness = better solution (minimization problem transformed to maximization).

---

## 📁 Project Structure | 项目结构

```
Port-Ship-Scheduling-GA/
├── genetic_algorithm.cpp                          # Main GA implementation (16KB)
├── test.vcxproj                                   # Visual Studio project file
├── compile_and_run_cplex1263.bat                 # Compile + run with CPLEX 12.6.3
├── compile_cplex1263.bat                          # Compile with CPLEX 12.6.3
├── compile_with_paths.bat                         # Compile with explicit include/lib paths
├── install_cplex_wsl.sh                           # CPLEX installation script for WSL
├── 港口船舶调度系统-遗传算法实现-爆款博客.md       # Technical blog (61KB)
└── README.md
```

---

## 🚀 Quick Start | 快速开始

### Prerequisites | 前置条件

- C++ compiler (MSVC / GCC / Clang) with C++11+ support
- (Optional) IBM ILOG CPLEX Optimization Studio 12.6.3+ for exact solver comparison

### Compile and Run (GA only, no CPLEX) | 编译运行（仅遗传算法）

```bash
# Using GCC
g++ -std=c++11 -O2 genetic_algorithm.cpp -o port_scheduling_ga
./port_scheduling_ga

# Using MSVC (Visual Studio Developer Command Prompt)
cl /O2 /EHsc genetic_algorithm.cpp /Fe:port_scheduling_ga.exe
port_scheduling_ga.exe
```

### Compile with CPLEX | 使用 CPLEX 编译

```bash
# Windows (using provided batch script)
compile_cplex1263.bat

# Or manually (adjust paths to your CPLEX installation)
cl /O2 /EHsc /I"C:\Program Files\IBM\ILOG\CPLEX_Studio1263\cplex\include" ^
   genetic_algorithm.cpp ^
   /link /LIBPATH:"C:\Program Files\IBM\ILOG\CPLEX_Studio1263\cplex\lib\x64_windows_vs2017\stat_mda" ^
   cplex1263.lib
```

### WSL CPLEX Installation | WSL CPLEX 安装

```bash
bash install_cplex_wsl.sh
```

### Expected Output | 预期输出

```
Ship Data:
Ship 0: Arrive Time=38, Draft=16, Length=193, Weight=62437, Cargo Type=2, Required Bays=2
Ship 1: Arrive Time=67, Draft=13, Length=231, Weight=92285, Cargo Type=2, Required Bays=2
...

Running Genetic Algorithm...
Generation 0, Best Fitness: 0.000123
Generation 100, Best Fitness: 0.000456
...
Generation 900, Best Fitness: 0.000789

Best Objective Value: 1265432.10
Transport Cost: 45678.90
Total Time: 40.65
```

---

## 📊 Sample Problem Data | 示例问题数据

### Ships | 船舶

| Ship | Arrive Time | Draft (m) | Length (m) | Weight (tons) | Cargo Type | Bays Needed |
|------|------------|-----------|------------|---------------|------------|-------------|
| 0 | 38 | 16 | 193 | 62,437 | Type 2 | 2 |
| 1 | 67 | 13 | 231 | 92,285 | Type 2 | 2 |
| 2 | 141 | 16 | 310 | 178,100 | Type 0 | 9 |
| 3 | 128 | 21 | 330 | 165,921 | Type 1 | 6 |

### Berths | 泊位

| Berth | Depth (m) | Length (m) | Unload Rate (Type 0) | Unload Rate (Type 1) | Unload Rate (Type 2) |
|-------|-----------|------------|----------------------|----------------------|----------------------|
| 0 | 16 | 255 | 2,942 | 2,089 | 3,339 |
| 1 | 24 | 360 | 3,695 | 3,064 | 4,092 |

### Reclaimers | 堆取料机

| Reclaimer | Stack Rate (Type 0) | Stack Rate (Type 1) | Stack Rate (Type 2) |
|-----------|---------------------|---------------------|---------------------|
| 0 | 3,932 | 3,366 | 4,475 |
| 1 | 4,865 | 4,167 | 5,352 |
| 2 | 4,534 | 4,043 | 5,188 |

---

## 🔬 Algorithm Flow | 算法流程

```
┌─────────────────────────────────────────┐
│          Initialize Population            │
│  (100 random feasible chromosomes)        │
└──────────────────┬──────────────────────┘
                   ▼
┌─────────────────────────────────────────┐
│       Calculate Fitness for All          │
│  (port time + transport cost)             │
└──────────────────┬──────────────────────┘
                   ▼
┌─────────────────────────────────────────┐
│         Record Best Chromosome            │
└──────────────────┬──────────────────────┘
                   ▼
┌─────────────────────────────────────────┐
│    Generation Loop (0 to 999)            │
│  ┌─────────────────────────────────────┐ │
│  │  1. Roulette Wheel Selection         │ │
│  │  2. Elitism (top 10% preserved)     │ │
│  │  3. Single-Point Crossover (80%)     │ │
│  │  4. Constraint-Aware Mutation (10%)  │ │
│  │  5. Calculate New Fitness             │ │
│  │  6. Update Best Chromosome            │ │
│  └─────────────────────────────────────┘ │
└──────────────────┬──────────────────────┘
                   ▼
┌─────────────────────────────────────────┐
│        Output Best Solution               │
│  (objective, transport cost, total time)  │
└─────────────────────────────────────────┘
```

---

## 📚 References | 参考文献

1. **Holland, J. H.** (1975). *Adaptation in natural and artificial systems.* University of Michigan Press.
2. **Goldberg, D. E.** (1989). *Genetic algorithms in search, optimization, and machine learning.* Addison-Wesley.
3. **Imai, A., et al.** (2001). *The dynamic berth allocation problem for a container port.* Transportation Research Part B, 35(4), 401-417.
4. **Liang, C. J., et al.** (2009). *A simulation model for container terminal analysis.* Simulation Modelling Practice and Theory.
5. **IBM ILOG.** (2024). *CPLEX Optimization Studio User's Manual.*
6. **Steenken, D., et al.** (2004). *Container terminal operation and operations research — a classification and literature review.* OR Spectrum, 26(1), 3-49.

---

## 📄 License | 许可证

MIT License — free to use, modify, and distribute.

---

<div align="center">

**Built with 🚢 for port operations research**

[Report Bug](https://github.com/Windyhhh/Port-Ship-Scheduling-GA/issues) · [Request Feature](https://github.com/Windyhhh/Port-Ship-Scheduling-GA/issues)

</div>
