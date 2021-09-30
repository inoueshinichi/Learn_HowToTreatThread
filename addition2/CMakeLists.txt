cmake_minimum_required(VERSION 3.16.3)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# pthread
find_package(Threads REQUIRED)

# -----------------------------------------------

# list_01 スレッドの生成
add_executable(LIST201
    list2_01.cpp
)
list(APPEND Targets LIST201)



# pthread
foreach(target IN LISTS Targets)
    target_link_libraries(${target} 
        PRIVATE Threads::Threads
    )
endforeach()