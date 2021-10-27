cmake_minimum_required(VERSION 3.16.3)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# pthread
find_package(Threads REQUIRED)

# -----------------------------------------------

# list2_01 データ競合とミューテックス
add_executable(LIST201
    list2_01.cpp
)
list(APPEND Targets LIST201)


# list2_02 ミューテックスによるデッドロック
add_executable(LIST202
    list2_02.cpp
)
list(APPEND Targets LIST202)



# pthread
foreach(target IN LISTS Targets)
    target_link_libraries(${target} 
        PRIVATE Threads::Threads
    )
endforeach()