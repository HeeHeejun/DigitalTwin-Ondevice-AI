from setuptools import setup, Extension
import pybind11
import os
#python setup.py build_ext --inplace
lidar_include_dir = "3rdparty/ridar_sdk/include"
lidar_library_dir = "3rdparty/ridar_sdk"
lidar_library = "libsl_lidar_sdk.a"

ext_modules = [
    Extension(
        'sensor',
        sources=['core/ridar.cc','core/binding.cc'],
        include_dirs=[
            pybind11.get_include(),
            'include',  # 프로젝트 헤더 파일 경로
            lidar_include_dir,
        ],
        library_dirs=[
            lidar_library_dir,
        ],
        libraries=[
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