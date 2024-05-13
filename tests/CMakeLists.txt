set(BINARY ${CMAKE_PROJECT_NAME}_test)

# getting GTest
enable_testing()

# building tests
file(GLOB_RECURSE test_sources "*.h*" "*.cpp*")
add_executable(${BINARY} ${test_sources})
add_test(NAME ${BINARY} COMMAND ${BINARY})

# linking with gtest and implemented classes
target_link_libraries(${BINARY} PRIVATE ${CMAKE_PROJECT_NAME} gtest gmock easyloggingpp)