add_executable(
    image_processor
    src/image_processor.cpp
    src/io.h
    src/io.cpp
    src/image.h
    src/image.cpp
    src/parser.h
    src/parser.cpp
    src/filters/filter.h
    src/filters/filter.cpp
        src/pixel.cpp src/bmp_chunks.h src/filters/crop.cpp src/filters/crop.h src/filters/edge_detection.cpp src/filters/edge_detection.h src/filters/gaussian_blur.cpp src/filters/gaussian_blur.h src/filters/grayscale.cpp src/filters/grayscale.h src/filters/negative.cpp src/filters/negative.h src/filters/sharpening.cpp src/filters/sharpening.h src/controller.cpp)
