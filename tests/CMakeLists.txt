#allocators
add_executable(test_linear test_linear.c)
target_link_libraries(test_linear zoner)
add_test(NAME ZonLinearTest COMMAND test_linear)

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
