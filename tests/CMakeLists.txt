#allocators
add_executable(test_heap test_heap.c)
target_link_libraries(test_heap zoner)
target_compile_options(test_heap PRIVATE -g)
add_test(NAME ZonHeapTest COMMAND test_heap)

add_executable(test_arena test_arena.c)
target_link_libraries(test_arena zoner)
add_test(NAME ZonArenaTest COMMAND test_arena)

add_executable(test_stack test_stack.c)
target_link_libraries(test_stack zoner)
add_test(NAME ZonStackTest COMMAND test_stack)

#pools
add_executable(test_flpool test_flpool.c)
target_link_libraries(test_flpool zoner)
add_test(NAME ZonFLPoolTest COMMAND test_flpool)
