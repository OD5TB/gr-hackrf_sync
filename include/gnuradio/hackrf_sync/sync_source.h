/* -*- c++ -*- */
/*
 * Copyright 2026 OD5TB.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_HACKRF_SYNC_SYNC_SOURCE_H
#define INCLUDED_HACKRF_SYNC_SYNC_SOURCE_H

#include <gnuradio/sync_block.h>
#include <gnuradio/hackrf_sync/api.h>

namespace gr {
namespace hackrf_sync {

class HACKRF_SYNC_API sync_source : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<sync_source> sptr;

    // Updated to match the new YAML and .cc logic
    static sptr make(std::string serial, 
                     double freq, 
                     double samp_rate, 
                     double bandwidth, 
                     int if_gain, 
                     int bb_gain, 
                     bool rf_amp, 
                     bool hw_sync,
                     bool clkout,  // Added
                     bool clkin);  // Added
};

} // namespace hackrf_sync
} // namespace gr

#endif /* INCLUDED_HACKRF_SYNC_SYNC_SOURCE_H */
