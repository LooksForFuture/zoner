add_executable(test_linear test_linear.c)
target_link_libraries(test_linear zoner)
add_test(NAME LinearAllocTest COMMAND test_linear)
