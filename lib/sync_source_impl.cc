/* -*- c++ -*- */
/*
 * Copyright 2026 OD5TB.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "sync_source_impl.h"
#include <gnuradio/io_signature.h>
#include <iostream>
#include <unistd.h> // For usleep

namespace gr {
namespace hackrf_sync {

// Updated 'make' function to include all 7 parameters
sync_source::sptr
sync_source::make(std::string serial, double freq, double samp_rate, double bandwidth, 
                  int if_gain, int bb_gain, bool rf_amp, bool hw_sync)
{
    return gnuradio::make_block_sptr<sync_source_impl>(
        serial, freq, samp_rate, bandwidth, if_gain, bb_gain, rf_amp, hw_sync);
}

sync_source_impl::sync_source_impl(std::string serial, double freq, double samp_rate, 
                                   double bandwidth, int if_gain, int bb_gain, 
                                   bool rf_amp, bool hw_sync)
    : gr::sync_block("sync_source",
                     gr::io_signature::make(0, 0, 0),
                     gr::io_signature::make(1, 1, sizeof(gr_complex))),
      d_device(nullptr),
      d_serial(serial),
      d_freq(freq),
      d_samp_rate(samp_rate),
      d_bandwidth(bandwidth),
      d_if_gain(if_gain),
      d_bb_gain(bb_gain),
      d_rf_amp(rf_amp),
      d_hw_sync(hw_sync),
      d_is_running(false)
{
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

    // --- APPLY RADIO SETTINGS ---
    hackrf_set_sample_rate(d_device, d_samp_rate);
    hackrf_set_freq(d_device, (uint64_t)d_freq);
    hackrf_set_baseband_filter_bandwidth(d_device, (uint32_t)d_bandwidth);
    
    // Gain Stages
    hackrf_set_vga_gain(d_device, d_if_gain); // IF Gain (0-40)
    hackrf_set_lna_gain(d_device, d_bb_gain); // BB Gain (0-62)
    hackrf_set_amp_enable(d_device, d_rf_amp ? 1 : 0); // RF Pre-amp (14dB)

    // --- HARDWARE TRIGGER LOGIC ---
    if (d_hw_sync) {
        // MASTER MODE: Delay slightly so Slave can arm first
        usleep(50000); // 50ms delay
        hackrf_set_hw_sync_mode(d_device, 0); // Fire pulse
        std::cout << "HACKRF_SYNC: Master [" << d_serial.substr(d_serial.length()-4) 
                  << "] FIRING PULSE at " << d_freq/1e6 << " MHz..." << std::endl;
    } else {
        // SLAVE MODE: Arm and wait
        hackrf_set_hw_sync_mode(d_device, 1); // Wait for pulse
        std::cout << "HACKRF_SYNC: Slave [" << d_serial.substr(d_serial.length()-4) 
                  << "] ARMED & WAITING FOR PIN 16..." << std::endl;
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

int sync_source_impl::_hackrf_callback(hackrf_transfer* transfer)
{
    // IQ processing logic will go here
    return 0;
}

int sync_source_impl::work(int noutput_items,
                           gr_vector_const_void_star& input_items,
                           gr_vector_void_star& output_items)
{
    return noutput_items;
}

} // namespace hackrf_sync
} // namespace gr
