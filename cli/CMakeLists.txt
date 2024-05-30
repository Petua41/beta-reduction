set(BINARY beta-red)

enable_testing()

file(GLOB lib_sources "*.h*" "*.cpp*")

add_executable(${BINARY} ${lib_sources})

target_include_directories(${BINARY} PRIVATE "src")
target_link_libraries(${BINARY} PRIVATE ${Boost_LIBRARIES} ${CMAKE_PROJECT_NAME})

target_include_directories(${BINARY} PRIVATE ${Boost_INCLUDE_DIRS})
