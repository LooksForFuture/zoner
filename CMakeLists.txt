cmake_minimum_required(VERSION 3.10)
project(zoner
	LANGUAGES C
)

set(LIB_NAME zoner)

add_library(${LIB_NAME} STATIC
	#allocators
	src/zon_heap.c
	src/zon_arena.c
	src/zon_stack.c

	#pools
	src/zon_flpool.c
)
target_include_directories(${LIB_NAME}
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
set_property(TARGET ${LIB_NAME} PROPERTY C_STANDARD 11) #setting c standard to c11
set_property(TARGET ${LIB_NAME} PROPERTY C_STANDARD_REQUIRED ON)
set_property(TARGET ${LIB_NAME} PROPERTY C_EXTENSIONS OFF)

target_compile_options(${LIB_NAME} PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

enable_testing()
add_subdirectory(tests)
