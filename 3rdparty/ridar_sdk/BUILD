cc_library(
    name = "sl_lidar_sdk",
    srcs = ["libsl_lidar_sdk.a"],  # 실제 .a 파일 경로에 맞게 수정
    hdrs = glob(["include/**/*.h"]),  # 헤더 파일 경로
    includes = ["include"],
    visibility = ["//visibility:public"],
    linkopts = ["-lpthread"],  # 필요한 링크 옵션
)
