#ifndef INCLUDED_HACKRF_SYNC_SYNC_SOURCE_H
#define INCLUDED_HACKRF_SYNC_SYNC_SOURCE_H

#include <gnuradio/hackrf_sync/api.h>
#include <gnuradio/sync_block.h>
#include <string> // --- ADDED THIS ---

namespace gr {
namespace hackrf_sync {

class HACKRF_SYNC_API sync_source : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<sync_source> sptr;

    // --- UPDATED: Use std::string and add hw_sync ---
    static sptr make(std::string serial="", bool hw_sync=false);
};

} // namespace hackrf_sync
} // namespace gr

#endif /* INCLUDED_HACKRF_SYNC_SYNC_SOURCE_H */
