cmake_minimum_required(VERSION 3.10)

project(SimpleSock)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdl/cmake)

add_executable(SimpleSock main.cpp socklib_generic.cpp)

target_compile_features(SimpleSock PRIVATE cxx_std_17)

target_link_libraries(SimpleSock PRIVATE SDL2::SDL2 SDL2::SDL2main)

#target_link_libraries(SimpleSock PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2.lib ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2main.lib)

#target_link_libraries(SimpleSock PRIVATE SDL2::SDL2 SDL2::SDL2main)

# target_compile_options(SimpleSock PRIVATE -fsanitize=address)
# target_link_options(SimpleSock PRIVATE -fsanitize=address)

set(CMAKE_BUILD_TYPE Debug)

if (UNIX)
	target_sources(SimpleSock PRIVATE socklib_posix.cpp)
endif (UNIX)

if (WIN32)
	target_sources(SimpleSock PRIVATE socklib_win32.cpp)
endif (WIN32)

target_sources(SimpleSock PRIVATE pool.cpp)