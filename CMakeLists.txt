cmake_minimum_required(VERSION 2.7)
project(Lightswing)

#set the compiler option
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fprofile-arcs -std=c++11 ")
set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} -O0 -fprofile-arcs")
# set the executable path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build/bin)

#add the library : thread
find_package(Threads REQUIRED)

## build 
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
add_executable(demo ${SOURCE_FILES})

target_link_libraries(demo ${CMAKE_THREAD_LIBS_INIT})
