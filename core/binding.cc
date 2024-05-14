#include "camera.h"
#include "ridar.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(sensor, m) {
    py::class_<camera>(m, "camera")
        .def(py::init<>())
        .def("do_capture", &camera::do_capture)
        .def("pop_cap", &camera::pop_cap);

    py::class_<ridar>(m, "ridar")
        .def(py::init<>())
        .def("scan_start", &ridar::scan_start)
        .def("scan_data", &ridar::scan_data)
        .def("get_data", &ridar::get_data);
}