cmake_minimum_required(VERSION 3.16.3)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# pthread
find_package(Threads REQUIRED)

# -----------------------------------------------

# list_01 スレッドの生成
add_executable(LIST01
    list_01.cpp
)
list(APPEND Targets LIST01)

# list_02 異常終了
add_executable(LIST02
    list_02.cpp
)
list(APPEND Targets LIST02)

# list_03 処理の結果をセットする参照を用意する
add_executable(LIST03
    list_03.cpp
)
list(APPEND Targets LIST03)

# list_04 promise/futureで結果を引き取る
add_executable(LIST04
    list_04.cpp
)
list(APPEND Targets LIST04)

# list_05 packaged_task/futureで戻り値を引き取る
add_executable(LIST05
    list_05.cpp
)
list(APPEND Targets LIST05)

# list_06 async()で着火し、結果を引き取る
add_executable(LIST06
    list_06.cpp
)
list(APPEND Targets LIST06)

# list_07 例外をつかまえそこねる
add_executable(LIST07
    list_07.cpp
)
list(APPEND Targets LIST07)

# list_08 exception_ptrで例外を引き渡す
add_executable(LIST08
    list_08.cpp
)
list(APPEND Targets LIST08)

# list_09 promiseは例外も引き渡せる
add_executable(LIST09
    list_09.cpp
)
list(APPEND Targets LIST09)

# list_10 package_task/async()での例外処理
add_executable(LIST10
    list_10.cpp
)
list(APPEND Targets LIST10)

# pthread
foreach(target IN LISTS Targets)
    target_link_libraries(${target} 
        PRIVATE Threads::Threads
    )
endforeach()


