# 변수 설정
CC := g++
CXXFLAGS := -Wall -std=c++11
RIDAR_FLAGS := -I/home/rpi_Hee/Desktop/DigitalTwin/core -I/home/rpi_Hee/Desktop/DigitalTwin/3rdparty/ridar_sdk/include
CAMERA_FLAGS := $(shell pkg-config --cflags opencv4) -I/home/rpi_Hee/Desktop/DigitalTwin/core

RIDAR_LDFLAGS := -L/home/rpi_Hee/Desktop/DigitalTwin/3rdparty/ridar_sdk -lsl_lidar_sdk -lpthread
CAMERA_LDFLAGS := $(shell pkg-config --libs opencv4)

# 소스 파일 및 대상 파일
RIDAR_SRC := /home/rpi_Hee/Desktop/DigitalTwin/test/ridar_Test.cc /home/rpi_Hee/Desktop/DigitalTwin/core/ridar.cc
CAMERA_SRC := /home/rpi_Hee/Desktop/DigitalTwin/test/camera_test.cc /home/rpi_Hee/Desktop/DigitalTwin/core/camera.cc

RIDAR_OBJ := $(patsubst %.cc,%.o,$(RIDAR_SRC))
CAMERA_OBJ := $(patsubst %.cc,%.o,$(CAMERA_SRC))

TESTS_DIR := /home/rpi_Hee/Desktop/DigitalTwin/out/tests
RIDAR_TARGET := $(TESTS_DIR)/ridar_test
CAMERA_TARGET := $(TESTS_DIR)/camera_test

# 기본 목표
.PHONY: all clean ridar_test camera_test tests setup

all: setup tests

setup:
	@mkdir -p $(TESTS_DIR)

ridar_test: setup $(RIDAR_TARGET)
	@rm -f $(RIDAR_OBJ)  # Delete object files after building

camera_test: setup $(CAMERA_TARGET)
	@rm -f $(CAMERA_OBJ)  # Delete object files after building

tests: ridar_test camera_test

$(RIDAR_TARGET): $(RIDAR_OBJ)
	$(CC) $^ $(RIDAR_LDFLAGS) -o $@

$(CAMERA_TARGET): $(CAMERA_OBJ)
	$(CC) $^ $(CAMERA_LDFLAGS) -o $@

%.o: %.cc
	$(CC) $(CXXFLAGS) $(RIDAR_FLAGS) $(CAMERA_FLAGS) -c $< -o $@

clean:
	@rm -f $(RIDAR_OBJ) $(CAMERA_OBJ)
	@rm -rf $(TESTS_DIR)
