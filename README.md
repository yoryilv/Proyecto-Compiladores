# C to x86-64 Compiler

A C++ compiler that translates a subset of C into **x86-64 assembly (AT&T syntax)** optimized for Linux.

This project implements all phases of a modern compiler: Lexical, Syntactic (with optimizations), and Semantic analysis, followed by Code Generation with intelligent register and memory management.

---

##  Key Features

### 🛠 Language Support
* **Data Types:** `int` (32-bit), `unsigned` (32-bit), `long` (64-bit), `float` (64-bit IEEE 754).
* **Structures:** Static multidimensional arrays (Row-major order).
* **Control Flow:** `if`, `else`, `while`, `for`.
* **Functions:** Recursive calls and parameters.

###  Optimizations
1. **Constant Folding:**
2. **Sethi-Ullman Algorithm:**
3. **Implicit Type Promotion:**

###  Robust Memory Management
* **Stack Alignment:** Strict 16-byte alignment (System V ABI) to ensure stability with external libraries.
* **Dynamic Offsets:** Precise memory addressing, distinguishing between 4-byte and 8-byte types to prevent corruption (Buffer Overflows).

---

##  Architecture

The project follows a sequential pipeline architecture:

1. **Scanner (`scanner.cpp`):** Tokenizes source code, handling literals and identifiers.
2. **Parser (`parser.cpp`):** Recursive descent parser that builds the AST and executes `tryFold` for optimization.
3. **Type Checker (`visitor.cpp`):** Validates types and pre-calculates stack memory requirements.
4. **Code Generator (`visitor.cpp`):** Traverses the AST to emit assembly, implementing Sethi-Ullman logic and instruction selection.

---

##  Web Visualizer

Includes a **Python (Flask) + GDB** debugging tool to visualize the compilation process.

* **Frontend:** Displays C code, generated Assembly, and CPU state side-by-step.
* **Visualization:** Live view of **Registers** (decimal/hex) and **Stack Memory** with `RBP`/`RSP` tracking.

---

##  Installation & Usage

### Prerequisites
* `g++`, `python3`, `flask`, `gdb`

### 1. Build the Compiler
```bash
g++ main.cpp scanner.cpp parser.cpp ast.cpp visitor.cpp token.cpp -o compilador
```

###  Web Visualizer

#### Launches a web interface to write code, compile, and debug the stack frame step-by-step.
Bash

#### Start the Flask Server
```bash
python3 app.py
```
