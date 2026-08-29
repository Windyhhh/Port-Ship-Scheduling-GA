<div align="center">

# 港口船舶调度 | Port-Ship-Scheduling-GA

### Genetic-algorithm port ship scheduling.

Berth allocation, stacker/reclaimer scheduling, storage assignment and job-order optimization via GA.

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Python](https://img.shields.io/badge/Python-3.8+-3776AB?logo=python&logoColor=white)](https://www.python.org/)

</div>

---

**Port-Ship-Scheduling-GA** solves port ship scheduling with a **genetic algorithm**, optimizing **berth allocation, stacker/reclaimer scheduling, storage assignment and job-order sequencing** — cutting transfer cost ~20% and port-stay time ~15% vs manual scheduling.

> [!NOTE]
> 中文项目：港口船舶调度系统——遗传算法实现泊位分配、堆取料机调度、储存位置分配与作业顺序优化。

---

## Features

- **GA global optimization** — robust multi-constraint scheduling.
- **Full problem coverage** — berth, stacker/reclaimer, storage, job order.
- **Measured gains** — ~20% lower transfer cost, ~15% shorter port-stay time.
- **Modular** — reusable scheduling framework.

---

## Quickstart

```bash
git clone https://github.com/Windyhhh/Port-Ship-Scheduling-GA.git
cd Port-Ship-Scheduling-GA

pip install -r requirements.txt

python src/main.py          # run the GA scheduler
```

---

## Project Structure

```
Port-Ship-Scheduling-GA/
├── src/                    # GA operators, scheduling modules
├── data/                   # berth / ship / yard data
├── results/                # optimized schedules
└── docs/                   # blog
```

---

## License

MIT — free to use, modify and distribute.
