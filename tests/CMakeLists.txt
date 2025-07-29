add_executable(test_linear test_linear.c)
target_link_libraries(test_linear zoner)
add_test(NAME ZonLinearTest COMMAND test_linear)

add_executable(test_flpool test_flpool.c)
target_link_libraries(test_flpool zoner)
add_test(NAME ZonFLPoolTest COMMAND test_flpool)
