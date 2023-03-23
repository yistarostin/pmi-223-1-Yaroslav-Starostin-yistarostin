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