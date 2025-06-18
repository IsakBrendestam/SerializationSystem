
APP_NAME := SerializationSystem

OUT_DIR := build
OUT := $(OUT_DIR)/$(APP_NAME)

CPP_VERSION := -std=c++20
GCC := clang++ $(CPP_VERSION)

all:
	$(GCC) src/Tests/Tests.cpp src/main.cpp -o $(OUT)

setup:
	mkdir -f $(OUT_DIR)

run:
	./$(OUT)

clean:
	rm -f ./$(OUT)
