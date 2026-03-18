# gr-hackrf_sync
**Phase-Coherent Radio Direction Finding (RDF) for GNU Radio 3.10**

This project provides a specialized GNU Radio block for **Interferometry** and **Direction Finding** at Freq  MHz. By locking the clocks and aligning the sample start-times of two HackRF One devices, we can measure the precise Phase Difference of an incoming signal.

## Hardware Configuration (Header P28)
For phase-coherence, the Master and Slave HackRFs must be physically synchronized:

1. **Clock Sync:** SMA cable from **CLK OUT** (Master) to **CLK IN** (Slave).
2. **Trigger Sync:** Jumper wire from **Pin 15 (Trigger Out)** on the Master to **Pin 16 (Trigger In)** on the Slave.
3. **Shared Ground:** Jumper wire between **Pin 2 (GND)** on both devices to ensure a common reference.

## Project Overview
This project enables synchronized radio reception using two HackRF One devices. 
By locking the 10MHz reference clocks (CLK IN/OUT) and using a physical 
trigger on Pin 16 and triger out pin 15, we can cross our fingers to measure the precise
Phase Difference at frequency selected.

## Calibration for the 180-Degree Needle
To prevent the direction needle from getting "stuck" at 20 degrees, 
you must avoid ADC Clipping. 

* **IF Gain:** 16 (Recommended)
* **BB Gain:** 20 (Recommended)
* **RF Amp:** OFF

When the gain is too high (BB Gain 30+), the wave peaks flatten out (Clipping). 
This "blinds" the phase math. Lowering the gain restores the curvy 
sine wave, allowing the needle to swing the full 90 degrees left and right.

## Installation
1. git clone https://github.com/OD5TB/gr-hackrf_sync.git
2. cd gr-hackrf_sync
3. mkdir build && cd build
4. cmake ..
5. make -j$(nproc)
6. sudo make install
7. sudo ldconfig


73
OD5RAL by OD5TB
