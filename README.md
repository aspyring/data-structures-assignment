# Lab 1: Array vs Linked-List Data Structures

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/aspyring/data-structures-assignment) [![License: MIT](https://img.shields.io/badge/license-MIT-blue)](LICENSE)

> Comparative C++ implementations of array vs. linked-list for sorting, filtering & frequency analysis with performance benchmarks.

---

## Table of Contents

1. [Introduction](#introduction)  
2. [Project Structure](#project-structure)  
3. [Getting Started](#getting-started)  
   - [Prerequisites](#prerequisites)  
   - [Installation](#installation)  
4. [Usage](#usage)  
5. [Benchmark Results](#benchmark-results)  
6. [Documentation](#documentation)  
7. [Contributing](#contributing)  
8. [License](#license)  
9. [Contact](#contact)  

---

## Introduction

Briefly describe the assignment goals:

- Implement Q1–Q3 using an array and your own linked-list classes  
- Compare performance (time & memory) between both versions  
- Deliver code, video demo, and write-up  

---

## Project Structure

```
data-structures-assignment/
├── array_version/           # Array-based implementation
│   ├── main.cpp
│   ├── array_utils.hpp
│   └── …
├── linkedlist_version/      # Linked-list implementation
│   ├── main.cpp
│   ├── linkedlist.hpp
│   └── …
├── data/
│   ├── transactions.csv
│   └── reviews.csv
├── docs/                    # Screenshots & diagrams
├── benchmarks/              # Benchmark results
│   └── results.md
├── LICENSE
└── README.md
```

---

## Getting Started

### Prerequisites

- C++ compiler (e.g. `g++` ≥ 7.0)  
- `make` (optional)  
- Git  

### Installation

```bash
# Clone the repo
git clone https://github.com/aspyring/data-structures-assignment.git
cd data-structures-assignment

# Build array version
cd array_version
g++ main.cpp -o array_app

# Build linked-list version
cd ../linkedlist_version
g++ main.cpp -o linkedlist_app
```  

---

## Usage

### Array Version

```bash
./array_app ../data/transactions.csv ../data/reviews.csv
```

### Linked-List Version

```bash
./linkedlist_app ../data/transactions.csv ../data/reviews.csv
```

---

## Benchmark Results

Detailed time & memory comparisons are in [benchmarks/results.md](benchmarks/results.md).

---

## Documentation

See the `docs/` folder for:

- Input/output screenshots  
- Complexity analysis  
- Workload matrix  

---

## Contributing

1. Fork this repo  
2. Create your feature branch:  
   ```bash
   git checkout -b feature/YourFeature
   ```  
3. Commit your changes:  
   ```bash
   git commit -m "Add YourFeature"
   ```  
4. Push to the branch:  
   ```bash
   git push origin feature/YourFeature
   ```  
5. Open a Pull Request  

---

## License

This project is licensed under the **MIT License**. See [LICENSE](LICENSE) for details.

---

## Contact

email: aspyringmc@gmail.com
Project Link: https://github.com/aspyring/data-structures-assignment  

---

