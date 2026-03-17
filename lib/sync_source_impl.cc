/* -*- c++ -*- */
/*
 * Copyright 2026 OD5TB.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "sync_source_impl.h"
#include <gnuradio/io_signature.h>
#include <iostream>

namespace gr {
namespace hackrf_sync {

// --- ADDED: The 'make' function that was missing ---
sync_source::sptr
sync_source::make(std::string serial, bool hw_sync)
{
    return gnuradio::make_block_sptr<sync_source_impl>(serial, hw_sync);
}

sync_source_impl::sync_source_impl(std::string serial, bool hw_sync)
    : gr::sync_block("sync_source",
                     gr::io_signature::make(0, 0, 0), // Source has 0 inputs
                     gr::io_signature::make(1, 1, sizeof(gr_complex))), // 1 Complex output
      d_device(nullptr),
      d_serial(serial),
      d_hw_sync(hw_sync),
      d_is_running(false)
{
    // Initialize HackRF library on block creation
    hackrf_init();
}

sync_source_impl::~sync_source_impl()
{
    stop();
    hackrf_exit();
}

bool sync_source_impl::start()
{
    if (hackrf_open_by_serial(d_serial.c_str(), &d_device) != HACKRF_SUCCESS) {
        std::cerr << "HACKRF_SYNC: Could not open device " << d_serial << std::endl;
        return false;
    }

    // Default settings for your 145MHz DF project
    hackrf_set_sample_rate(d_device, 1000000); // 1 Msps
    hackrf_set_baseband_filter_bandwidth(d_device, 750000);

    // --- HARDWARE TRIGGER LOGIC ---
    if (d_hw_sync) {
        // Slave mode: Pins 16 & 1 (GND) must be connected
        hackrf_set_hw_sync_mode(d_device, 1);
        std::cout << "HACKRF_SYNC: Slave [" << d_serial.substr(d_serial.length() > 4 ? d_serial.length()-4 : 0) 
                  << "] ARMED & WAITING FOR PIN 16..." << std::endl;
    } else {
        // Master mode: Fires pulse from Pin 15
        hackrf_set_hw_sync_mode(d_device, 0);
        std::cout << "HACKRF_SYNC: Master [" << d_serial.substr(d_serial.length() > 4 ? d_serial.length()-4 : 0) 
                  << "] FIRING PULSE..." << std::endl;
    }

    d_is_running = (hackrf_start_rx(d_device, _hackrf_callback, this) == HACKRF_SUCCESS);
    return d_is_running;
}

bool sync_source_impl::stop()
{
    if (d_device) {
        hackrf_stop_rx(d_device);
        hackrf_close(d_device);
        d_device = nullptr;
    }
    d_is_running = false;
    return true;
}

// Low-level HackRF callback
int sync_source_impl::_hackrf_callback(hackrf_transfer* transfer)
{
    // Logic for transferring IQ8 bytes to Complex Float would go here
    return 0;
}

int sync_source_impl::work(int noutput_items,
                           gr_vector_const_void_star& input_items,
                           gr_vector_void_star& output_items)
{
    // GNU Radio calls this to get samples
    return noutput_items;
}

} // namespace hackrf_sync
} // namespace gr
