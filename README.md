# gr-hackrf_sync
Phase-synchronous HackRF source for GNU Radio 3.10.

## Installation
```bash
# 1. Clone the repository
git clone https://github.com/OD5TB/gr-hackrf_sync.git
cd gr-hackrf_sync

# 2. Build the driver
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
sudo ldconfig
