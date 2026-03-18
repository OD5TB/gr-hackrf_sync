#include "sync_source_impl.h"
#include <gnuradio/io_signature.h>
#include <algorithm>

namespace gr {
namespace hackrf_sync {

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
      d_device(nullptr), d_serial(serial), d_freq(freq), d_samp_rate(samp_rate),
      d_bandwidth(bandwidth), d_if_gain(if_gain), d_bb_gain(bb_gain),
      d_rf_amp(rf_amp), d_hw_sync(hw_sync)
{
    hackrf_init();
    d_buffer.reserve(262144); // Pre-allocate buffer space
}

sync_source_impl::~sync_source_impl() {
    stop();
    hackrf_exit();
}

// --- The Hardware Callback (Fills the Buffer) ---
int sync_source_impl::_hackrf_callback(hackrf_transfer* transfer)
{
    sync_source_impl* obj = (sync_source_impl*)transfer->rx_ctx;
    std::lock_guard<std::mutex> lock(obj->d_mutex);

    for (size_t i = 0; i < transfer->valid_length; i += 2) {
        // Convert 8-bit signed integers to float complex (-1.0 to 1.0)
        float re = (float)((int8_t)transfer->buffer[i]) / 128.0f;
        float im = (float)((int8_t)transfer->buffer[i+1]) / 128.0f;
        obj->d_buffer.push_back(gr_complex(re, im));
    }
    return 0;
}

// --- The Work Function (Empties the Buffer into GRC) ---
int sync_source_impl::work(int noutput_items,
                           gr_vector_const_void_star& input_items,
                           gr_vector_void_star& output_items)
{
    gr_complex *out = (gr_complex *) output_items[0];
    std::lock_guard<std::mutex> lock(d_mutex);

    int items_to_copy = std::min(noutput_items, (int)d_buffer.size());
    
    if (items_to_copy > 0) {
        std::copy(d_buffer.begin(), d_buffer.begin() + items_to_copy, out);
        d_buffer.erase(d_buffer.begin(), d_buffer.begin() + items_to_copy);
    }

    return items_to_copy; 
}

bool sync_source_impl::start() {
    if (hackrf_open_by_serial(d_serial.c_str(), &d_device) != HACKRF_SUCCESS) return false;
    
    hackrf_set_sample_rate(d_device, d_samp_rate);
    hackrf_set_freq(d_device, (uint64_t)d_freq);
    hackrf_set_vga_gain(d_device, d_if_gain);
    hackrf_set_lna_gain(d_device, d_bb_gain);
    hackrf_set_amp_enable(d_device, d_rf_amp ? 1 : 0);

    if (d_hw_sync) {
        usleep(50000);
        hackrf_set_hw_sync_mode(d_device, 0); // Master
    } else {
        hackrf_set_hw_sync_mode(d_device, 1); // Slave
    }

    return (hackrf_start_rx(d_device, _hackrf_callback, this) == HACKRF_SUCCESS);
}

bool sync_source_impl::stop() {
    if (d_device) {
        hackrf_stop_rx(d_device);
        hackrf_close(d_device);
        d_device = nullptr;
    }
    return true;
}

} }
