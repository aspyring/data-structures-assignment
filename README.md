# Array vs Linked-List Data Structures

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/aspyring/data-structures-assignment) [![License: MIT](https://img.shields.io/badge/license-MIT-blue)](LICENSE)

> Comparative C++ implementations of array vs. linked-list for sorting, filtering & frequency analysis with performance benchmarks.

---

## Table of Contents

1. [Introduction](#introduction)
2. [Getting Started](#getting-started)
   - [Prerequisites](#prerequisites)
   - [Installation](#installation)
3. [Contributing](#contributing)
4. [License](#license)
5. [Contact](#contact)

---

## Introduction

The assignment goals:

- Implement Q1–Q3 using an array and your own linked-list classes
- Compare performance (time & memory) between both versions
- Deliver code, video demo, and documentation.

## Q1: How can you efficiently sort customer transactions by date and display the total number of transactions in both datasets?

## Q2: What percentage of purchases in the “Electronics” category were made using Credit Card payments?

## Q3: Which words are most frequently used in product reviews rated 1-star?

---



## Getting Started

### Prerequisites

- C++ compiler (e.g. `g++` ≥ 7.0)
- `CMake` 
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
