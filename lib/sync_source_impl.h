/* -*- c++ -*- */
/*
 * Copyright 2026 OD5TB.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_HACKRF_SYNC_SYNC_SOURCE_IMPL_H
#define INCLUDED_HACKRF_SYNC_SYNC_SOURCE_IMPL_H

#include <gnuradio/hackrf_sync/sync_source.h>
#include <hackrf.h>  // --- CRITICAL: Include the hardware library ---
#include <string>

namespace gr {
  namespace hackrf_sync {

    class sync_source_impl : public sync_source
    {
     private:
      // --- ADDED: Hardware Management Variables ---
      hackrf_device* d_device;
      std::string d_serial;
      bool d_hw_sync;      // Toggle for Hardware Triggering (Pin 16)
      bool d_is_running;

      // Callback function for HackRF data stream
      static int _hackrf_callback(hackrf_transfer* transfer);

     public:
      // Updated constructor to accept serial and sync toggle
      sync_source_impl(std::string serial, bool hw_sync);
      ~sync_source_impl();

      // Lifecycle overrides to control the SDR hardware
      bool start() override;
      bool stop() override;

      // The work function where GNU Radio processes samples
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      ) override;
    };

  } // namespace hackrf_sync
} // namespace gr

#endif /* INCLUDED_HACKRF_SYNC_SYNC_SOURCE_IMPL_H */
