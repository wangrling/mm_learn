cmake_minimum_required(VERSION 3.10)

add_executable(svpng code/svpng/svpng_test.cpp)

add_executable(endian code/endian/endian_test.cpp)

# 包含skia所有的头文件。
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/android")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/atlastext")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/c")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/codec")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/config")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/core")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/docs")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/effects")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/encode")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/gpu")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/pathops")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/ports")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/private")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/svg")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/third_party")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include/utils")
include_directories("${PROJECT_SOURCE_DIR}/code/skia/include")

# 包含skia四个动态库
add_library(libskia SHARED IMPORTED)
set_target_properties(libskia PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/so/libskia.so)

add_library(libskottie SHARED IMPORTED)
set_target_properties(libskottie PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/so/libskottie.so)

add_library(libsksg SHARED IMPORTED)
set_target_properties(libsksg PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/so/libsksg.so)

add_library(libskshaper SHARED IMPORTED)
set_target_properties(libskshaper PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/so/libskshaper.so)

add_executable(SkiaBitmap_test code/skia/tests/SkBitmap_test.cpp)
target_link_libraries(SkiaBitmap_test libskia libskottie libsksg libskshaper)

find_package(SDL2 REQUIRED)

find_package(OpenGL REQUIRED)

add_executable(SkiaSDL_test code/skia/tests/SkiaSDL_test.cpp)
target_link_libraries(SkiaSDL_test libskia libskottie libsksg libskshaper ${SDL2_LIBRARIES} ${OPENGL_LIBRARY})

add_executable(SkiaLottie_test code/skia/tests/SkiaLottie_test.cpp)
target_link_libraries(SkiaLottie_test libskia libskottie libsksg libskshaper)