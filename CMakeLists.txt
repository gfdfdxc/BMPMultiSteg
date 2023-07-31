cmake_minimum_required(VERSION 3.24)
project(BMPMultiSteg C)

set(CMAKE_C_STANDARD 99)

include_directories(src)
include_directories(3rd)

aux_source_directory(src SRC_FILES)
aux_source_directory(3rd 3RD_FILES)

add_executable(BMPMultiSteg ${SRC_FILES} ${3RD_FILES})

install(TARGETS BMPMultiSteg)
