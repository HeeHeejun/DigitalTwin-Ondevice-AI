cc_test(
    name = "ridar_test",
    srcs = ["ridar_Test.cc"],  # 테스트 파일만 포함
    copts = ["-Wall", "-std=c++11"],
    deps = [
        "//core:ridar",  # ridar 라이브러리 참조
    ],
)

cc_test(
    name = "camera_test",
    srcs = ["camera_test.cc"],  # 테스트 파일만 포함
    copts = ["-Wall", "-std=c++11"],
    deps = [
        "//core:camera",  # camera 라이브러리 참조
    ],
)
