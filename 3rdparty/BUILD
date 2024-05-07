cc_library(
    name = "opencv",
    hdrs = glob(["/usr/local/include/opencv4/**/*.hpp", "/usr/local/include/opencv4/**/*.h"]),
    includes = ["/usr/local/include/opencv4"],
    linkopts = [
        "-L/usr/local/lib", "-lopencv_core", "-lopencv_imgproc", "-lopencv_highgui",
        "-lopencv_imgcodecs",  # 필요한 경우 추가 OpenCV 모듈
    ],
    visibility = ["//visibility:public"],
)
