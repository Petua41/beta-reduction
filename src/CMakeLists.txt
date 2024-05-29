set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE lib_sources "*/*.h*" "*/*.cpp*" "*/*.cc*")

add_library(${BINARY} STATIC ${lib_sources})

set_property(TARGET ${BINARY} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(${BINARY} PUBLIC ".")
target_include_directories(${BINARY} SYSTEM PUBLIC "../vendor/easyloggingpp/src")
target_link_libraries(${BINARY} PUBLIC easyloggingpp)

target_include_directories(${BINARY} PUBLIC ${Boost_INCLUDE_DIRS})