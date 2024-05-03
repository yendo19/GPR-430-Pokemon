cmake_minimum_required(VERSION 3.10)

project(Pokeman)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdl/cmake)
find_package(SDL2_TTF REQUIRED CONFIG REQUIRED COMPONENTS SDL2 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_ttf/cmake)

file(
	GLOB_RECURSE POKE_INC "inc/*.h"
)

file(
	GLOB_RECURSE POKE_SRC "inc/*.cpp"
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/)

install(FILES ${POKE_INC} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Pokeman)
add_executable(Pokeman main.cpp socklib.h socklib_generic.cpp ${POKE_SRC} ${POKE_INC})

target_compile_features(Pokeman PRIVATE cxx_std_17)

target_link_libraries(Pokeman PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_ttf::SDL2_ttf)

set(CMAKE_BUILD_TYPE Debug)

if (UNIX)
	target_sources(Pokeman PRIVATE socklib_posix.cpp)
endif (UNIX)

if (WIN32)
	target_sources(Pokeman PRIVATE socklib_win32.cpp)
	#Just copy the dang dll
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdl/lib/x64/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR})
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_ttf/lib/x64/SDL2_ttf.dll DESTINATION ${CMAKE_BINARY_DIR})
endif (WIN32)

target_sources(Pokeman PRIVATE pool.cpp)