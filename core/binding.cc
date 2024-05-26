#include "ridar.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(sensor, m) {
    py::class_<ridar>(m, "ridar")
        .def(py::init<>())
        .def("scan_start", &ridar::scan_start)
        .def("scan_data", &ridar::scan_data)
        .def("get_data", &ridar::get_data)
        .def("get_distance_from_anlge", &ridar::get_distance_from_anlge);
}