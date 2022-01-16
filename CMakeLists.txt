cmake_minimum_required(VERSION 3.20)
project(chess)

include_directories(src)
include_directories(include)

set(CMAKE_CXX_STANDARD 20)

set(SFML_INCLUDE_DIR "include/SFML")
set(SFML_LIBRARY_DIR "lib/SFML/lib")

link_directories(SFML_LIBRARY_DIR)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

set(SOURCE_FILES main.cpp)
set(LIBRARIES sfml-graphics sfml-audio sfml-window sfml-system source)

add_subdirectory(tests)
add_subdirectory(src)
add_subdirectory(benchmarks)

add_executable(console ${SOURCE_FILES})
target_link_libraries(console ${LIBRARIES})

add_executable(macos_app MACOSX_BUNDLE ${SOURCE_FILES})
target_link_libraries(macos_app sfml-graphics sfml-window sfml-system)