set(BINARY ${CMAKE_PROJECT_NAME}_cli)

file(GLOB_RECURSE lib_sources "*.h*" "*.cpp*")

include_directories("..")

add_executable(${BINARY} ${lib_sources})

target_link_libraries(${BINARY} PRIVATE ${Boost_LIBRARIES} ${CMAKE_PROJECT_NAME})