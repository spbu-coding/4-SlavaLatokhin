cmake_minimum_required(VERSION 3.7)
project(tests LANGUAGES C)

set(CMAKE_C_STANDARD 99)

add_executable(converter srs/converter.c srs/bmp_func.c srs/qdbmp.c)
add_executable(comparer srs/comparer.c srs/bmp_func.c)


