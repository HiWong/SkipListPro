cmake_minimum_required(VERSION 3.9)
project(skiplist)

set(CMAKE_CXX_STANDARD 11)

enable_testing()

add_executable(skiplist src/main.cc src/node.h src/skiplist.h src/random.h)

# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DBUILD_MEMLEAK=ON -G Ninja -B build 
# ninja -C build 
# ctest --test-dir build --rerun-failed --output-on-failure
set(BUILD_FLAGS "-DDEBUG -g -Wall -Wpedantic" )
set(LD_FLAGS "")

if(BUILD_MEMLEAK)
    set(BUILD_FLAGS "${BUILD_FLAGS} -fsanitize=address")
    set(LD_FLAGS "-fsanitize=address")
endif()


target_compile_options(skiplist PRIVATE ${BUILD_FLAGS})
target_link_options(skiplist PRIVATE  ${LD_FLAGS})

add_test(skip_list skiplist COMMAND skiplist WORKING_DIRECTORY
    $(CMAKE_CURRENT_SOURCE_DIR))
