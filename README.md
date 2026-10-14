# 💻 20-Bit CPU Simulator

A software-based CPU simulator developed as an architecture modeling project (Taras Shevchenko National University of Kyiv). This application models the fundamental principles of a processor's operation, including data storage in registers, clock-cycle execution, and a 3-address instruction format.

## ⚙️ Technical Specifications

* **Architecture:** 3-address format (the result of the operation is stored in the first operand).
* **Word Size:** 20-bit architecture for all registers, the stack, and command operands.
* **System Components:**
  * `IR` (Instruction Register) — Stores the text record of the currently executing command.
  * `R1`, `R2`, `R3` — General-purpose registers.
  * `ST` (Stack) — Fixed-size 20-bit stack representation.
  * `PS` (Program Status) — Status register holding the sign of the last operation's result (`0` for positive, `1` for negative).
  * `PC` (Program Counter) — Tracks the current instruction number.
  * `TC` (Tact Counter) — Tracks the clock cycles for the current instruction.

## 🛠 Instruction Set Architecture (ISA)

The processor reads instructions sequentially from a `commands.txt` file (one instruction per line). The simulator currently supports the following operations:

* `MOV [Operand1] [Operand2]` 
  * Loads a specific integer value or the contents of another register/stack into `Operand1`.
* `INV [Operand1] [Operand2] [Flag]` 
  * Inverts specific bits of `Operand1` based on the `Flag` value, referencing `Operand2`:
    * If `Flag = 0`: Inverts only the **even** bits.
    * If `Flag = 1`: Inverts only the **odd** bits.

### Execution

Create a `commands.txt` file in the root directory and add your instructions. Example:
   ```text
   MOV R1 5
   MOV R3 10
   INV R1 ST 0
   MOV R2 45
   INV R1 R2 1