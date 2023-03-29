# add_executable(
        #image_processor
        #src/image_processor.cpp
        #src/io.h
        #src/io.cpp
        #src/image.h
        #src/image.cpp
        #src/parser.h
        #src/parser.cpp
        #src/filters/filter.h
        #src/filters/filter.cpp
        #src/pixel.cpp
        #src/bmp_chunks.h
        #src/filters/crop.cpp src/filters/crop.h src/filters/edge_detection.cpp src/filters/edge_detection.h src/filters/gaussian_blur.cpp src/filters/gaussian_blur.h src/filters/grayscale.cpp src/filters/grayscale.h src/filters/negative.cpp src/filters/negative.h src/filters/sharpening.cpp src/filters/sharpening.h src/controller.cpp
#)
cmake_minimum_required(VERSION 3.8)

project("Image Processor"
        VERSION 1.0
        LANGUAGES CXX
        )

set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB filters filters/*.cpp)
set(LIB_SRC
        bmp_chunks.h
        controller.cpp
        controller.h
        image.h
        image.cpp
        io.cpp
        io.h
        parser.cpp
        parser.h
        pixel.cpp
        pixel.h
        ${filters}
        )

add_library(
        image_processor_lib OBJECT
        ${LIB_SRC}
)

add_executable(
        image_processor
        image_processor.cpp
)
target_link_libraries(image_processor image_processor_lib)
