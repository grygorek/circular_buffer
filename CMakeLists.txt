cmake_minimum_required(VERSION 3.18)

project(circ_buffer)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
find_package(GTest REQUIRED)
if(NOT GTEST_FOUND)
  message(FATAL_ERROR "GTest not found")
endif()

if(MSVC)
 foreach(flag CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
   string(REPLACE "/MD" "/MT" ${flag} "${${flag}}")
 endforeach()
endif()

message(${CMAKE_CXX_FLAGS} "\n" ${CMAKE_CXX_FLAGS_DEBUG} "\n" ${CMAKE_CXX_FLAGS_RELEASE}
           "\n" ${CMAKE_CXX_FLAGS_MINSIZEREL} "\n" ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})

include(GoogleTest)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(TEST_PROJECT ${CMAKE_PROJECT_NAME}_test)

add_executable(${TEST_PROJECT} tests/circ_buffer_test.cpp)
if(MSVC)
  set_property(TARGET ${TEST_PROJECT} PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
target_link_libraries(${TEST_PROJECT} GTest::GTest GTest::Main)

gtest_discover_tests(${TEST_PROJECT})
