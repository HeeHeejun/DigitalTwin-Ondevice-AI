# ridar 라이브러리 정의
cc_library(
    name = "ridar",
    srcs = ["ridar.cc"],
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
    deps = [
        "@ridar_sdk//:sl_lidar_sdk",  # ridar_sdk 라이브러리 종속성
    ],
    includes = [
        ".",  # 현재 디렉토리를 기준으로 포함 경로 추가
        "@ridar_sdk//include",
    ],
)

# camera 라이브러리 정의
cc_library(
    name = "camera",
    srcs = ["camera.cc"],
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
    deps = [
        "@opencv4//:opencv",  # OpenCV 라이브러리 종속성
    ],
    includes = [
        ".",
        "@opencv4//include/opencv4",  # OpenCV 헤더 파일의 include 경로
    ],
)

# core 라이브러리는 이제 ridar 및 camera 라이브러리에 의존합니다.
cc_library(
    name = "core",
    srcs = [],
    hdrs = glob(["*.h"]),
    visibility = ["//visibility:public"],
    deps = [
        ":ridar",
        ":camera",
    ],
)
