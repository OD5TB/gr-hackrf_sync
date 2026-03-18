#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/hackrf_sync/sync_source.h>

void bind_sync_source(py::module& m)
{
    using sync_source = gr::hackrf_sync::sync_source;
    py::class_<sync_source,
               gr::sync_block,
               std::shared_ptr<sync_source>>(m, "sync_source")

        .def(py::init(&sync_source::make),
             py::arg("serial"),
             py::arg("freq"),
             py::arg("samp_rate"),
             py::arg("bandwidth"),
             py::arg("if_gain"),
             py::arg("bb_gain"),
             py::arg("rf_amp"),
             py::arg("hw_sync"),
             "HackRF Sync Source Block");
}
