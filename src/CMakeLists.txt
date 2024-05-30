set(LIBRARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE lib_sources "*/*.h*" "*/*.cpp*" "*/*.cc*")

add_library(${LIBRARY} STATIC ${lib_sources})

set_property(TARGET ${LIBRARY} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(${LIBRARY} PUBLIC ".")
target_include_directories(${LIBRARY} SYSTEM PUBLIC "../vendor/easyloggingpp/src")
target_link_libraries(${LIBRARY} PUBLIC easyloggingpp)

target_include_directories(${LIBRARY} PUBLIC ${Boost_INCLUDE_DIRS})
