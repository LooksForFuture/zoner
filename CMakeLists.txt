cmake_minimum_required(VERSION 3.10)
project(zoner
	LANGUAGES C
)

set(LIB_NAME zoner)

add_library(${LIB_NAME} STATIC
	src/zon_linear.c
	src/zon_flpool.c
)
target_include_directories(${LIB_NAME}
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
set_property(TARGET ${LIB_NAME} PROPERTY C_STANDARD 99) #setting c standard to c99
set_property(TARGET ${LIB_NAME} PROPERTY C_STANDARD_REQUIRED ON)
set_property(TARGET ${LIB_NAME} PROPERTY C_EXTENSIONS OFF)

enable_testing()
add_subdirectory(tests)
