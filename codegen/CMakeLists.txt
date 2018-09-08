cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_FLAGS "${CXX_FLAGS} -O2 -Wall -Wextra -Wshadow -std=c++11")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS} -O0 -ggdb -Wall -Wextra -Wshadow -std=c++11")
set(SRC util.cpp)

add_library(bfcore SHARED ${SRC})

foreach(target_name test_bool test_less_signed test_is_negative crack_me
        test_integer test_compressed test_byte_array test_tree_byte_array)
    add_executable(${target_name} ${target_name}.cpp)
    target_link_libraries(${target_name} bfcore)
endforeach(target_name)

add_executable(bf2c bf_to_c.cpp)

add_executable(interpreter interpreter.cpp instructions.cpp)
target_link_libraries(interpreter bfcore)

add_executable(test_divmod test_divmod.cpp instructions.cpp)
target_link_libraries(test_divmod bfcore)
# add_executable(sort main.cpp SRC)
