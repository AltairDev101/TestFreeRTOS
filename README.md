# Test FreeRTOS:
This project demonstrates the operation of two tasks in a FreeRTOS environment:
1. **First task** performs analog data acquisition using the ADC and DMA .
2. **Second task** processes these data from a global buffer, using FreeRTOS notifications, and transmits them over the CAN bus.

Additionally, the code implements temperature calculation based on the **Steinmaier equation**.

## Project Structure

- **tasks/**  
  - `adc_task.c` — Measures the readings from the NTC temperature sensors and writes them to the global buffer.
  - `can_task.c` — Receives the data from the global buffer (through FreeRTOS notifications) and sends them via the CAN bus.
- **freertos.c**  
  - Here, the tasks are initialized.

**Clone the repository**:
```bash
git clone https://github.com/AltairDev101/TestFreeRTOS.git
```
**Build**:
```bash
mkdir build && cd build
cmake ..
cmake --build .
<<<<<<< HEAD
```