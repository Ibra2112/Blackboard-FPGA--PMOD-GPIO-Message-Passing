# UART-Based Prime Number Checker (FPGA Embedded C)

This project demonstrates how to interface with a UART peripheral using memory-mapped I/O on an FPGA. It receives an integer over UART, checks if it’s a prime number, and returns the result — all over a serial terminal.

## 📦 Overview

This program uses low-level register manipulation to configure and control UART1 on a Xilinx-based FPGA. It allows users to:
- Input a number via UART
- Check if it’s a prime number
- Return the result and closest prime if it’s not

## 🧠 Features

- UART1 configuration (manual control of baud rate, control and mode registers)
- Receive integer input via UART
- Prime number check algorithm
- Sends result back via UART
- Finds the closest prime less than the input (if input is not prime)

## 🔌 UART Register Map

| Register         | Address       | Description |
|------------------|---------------|-------------|
| `UART1_CR`       | `0xE0001000`  | Control register |
| `UART1_MR`       | `0xE0001004`  | Mode register |
| `UART1_BAUDGEN`  | `0xE0001018`  | Baud rate generator |
| `UART1_BAUDDIV`  | `0xE0001034`  | Baud rate divider |
| `UART1_SR`       | `0xE000102C`  | Status register |
| `UART1_DATA`     | `0xE0001030`  | Data register |

## ⚙️ Baud Rate Settings

The code sets the baud rate to **115200** using:
```c
#define BaudGen115200 0x7c
#define BaudDiv115200 6
```

## 🕹️ User Interaction

1. The user is prompted over UART to enter a number (≥ 2).
2. The input is read as a string and converted to an integer.
3. The system checks whether the number is prime:
   - If prime → displays confirmation.
   - If not → displays the nearest smaller prime number.
4. Loop repeats for next input.

## 🛠️ Dependencies

- `stdint.h` for integer types
- `stdio.h` for `sprintf`
- `sleep.h` for delay functions
- A serial terminal connected to UART1 (e.g., PuTTY, Tera Term)

## 🧮 Prime Checking Logic

```c
int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}
```

If not prime, the program searches backwards to find the nearest smaller prime.

## 🚀 Getting Started

1. Clone the repository and open the C file in Xilinx SDK or Vitis.
2. Connect UART1 on the FPGA board to a serial terminal at **115200 baud**, 8-N-1.
3. Build and program your FPGA with this code.
4. Open the terminal and interact with the program as prompted.

---

Let me know if you'd like an image, terminal demo, or ASCII diagram included!

