# Save this as write_readme.py
content = """# gr-hackrf_sync
Phase-synchronous HackRF source for GNU Radio 3.10 (RDF & Interferometry).

## Project Overview
This project enables synchronized radio reception using two HackRF One devices. 
By locking the 10MHz reference clocks (CLK IN/OUT) and using a physical 
trigger on Pin 16 and triger out pin 15, we can measure the precise 
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
"""


73
OD5RAL by OD5TB
