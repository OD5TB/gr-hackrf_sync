/* -*- c++ -*- */
/*
 * Copyright 2026 OD5TB.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_HACKRF_SYNC_SYNC_SOURCE_IMPL_H
#define INCLUDED_HACKRF_SYNC_SYNC_SOURCE_IMPL_H

#include <gnuradio/hackrf_sync/sync_source.h>
#include <libhackrf/hackrf.h>
#include <boost/circular_buffer.hpp>
#include <mutex>

namespace gr {
namespace hackrf_sync {

class sync_source_impl : public sync_source
{
private:
    // Hardware handles
    hackrf_device* d_device;
    std::string d_serial;
    
    // Radio parameters
    double d_freq;
    double d_samp_rate;
    double d_bandwidth;
    int d_if_gain;
    int d_bb_gain;
    bool d_rf_amp;
    bool d_hw_sync;
    bool d_clkout;  // Added for Clock Sync
    bool d_clkin;   // Added for Clock Sync
    bool d_is_running;

    // Data handling (The "Bridge" between HackRF and GNU Radio)
    std::mutex d_mutex;
    boost::circular_buffer<gr_complex> d_buffer;
    
    // Internal processing functions
    static int _hackrf_callback(hackrf_transfer* transfer);
    int process_samples(unsigned char* buffer, size_t length);

public:
    // Updated constructor signature to include clkout and clkin
    sync_source_impl(std::string serial, double freq, double samp_rate, 
                     double bandwidth, int if_gain, int bb_gain, 
                     bool rf_amp, bool hw_sync, bool clkout, bool clkin);
    ~sync_source_impl();

    // GNU Radio Lifecycle
    bool start() override;
    bool stop() override;

    // The main data loop
    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items) override;
};

} // namespace hackrf_sync
} // namespace gr

#endif /* INCLUDED_HACKRF_SYNC_SYNC_SOURCE_IMPL_H */
