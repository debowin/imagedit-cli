cmake_minimum_required(VERSION 3.5)
project(ImageCode)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        image.cpp
        image.h
        main.cpp
        pixel.cpp
        pixel.h
        stb_image.h
        stb_image_write.h)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

add_executable(image ${SOURCE_FILES})