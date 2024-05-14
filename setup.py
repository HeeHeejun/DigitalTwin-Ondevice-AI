from setuptools import setup, Extension
import pybind11
import os

# OpenCV의 헤더 및 라이브러리 경로
opencv_include_dir = "/usr/local/include/opencv4"
opencv_library_dir = "/usr/local/lib"

# SLAMTEC Lidar SDK 헤더 및 라이브러리 경로
lidar_include_dir = "3rdparty/ridar_sdk/include"
lidar_library_dir = "3rdparty/ridar_sdk"
lidar_library = "libsl_lidar_sdk.a"

ext_modules = [
    Extension(
        'sensor',
        sources=['core/camera.cc', 'core/ridar.cc','core/binding.cc'],
        include_dirs=[
            pybind11.get_include(),
            'include',  # 프로젝트 헤더 파일 경로
            opencv_include_dir,
            lidar_include_dir,
        ],
        library_dirs=[
            opencv_library_dir,
            lidar_library_dir,
        ],
        libraries=[
            'opencv_core', 'opencv_imgproc', 'opencv_highgui', 'opencv_imgcodecs', 'opencv_videoio', 'opencv_video',
            'sl_lidar_sdk'
        ],
        extra_objects=[
            os.path.join(lidar_library_dir, lidar_library)
        ],
        language='c++',
    ),
]

setup(
    name='sensor',
    version='0.1',
    ext_modules=ext_modules,
)