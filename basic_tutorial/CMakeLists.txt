cmake_minimum_required(VERSION 3.16.3)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# pthread
find_package(Threads REQUIRED)

# prime_counter
add_executable(PrimeCounter
    prime.hpp
    prime_counter.cpp
)

# C++ standard thread
add_executable(CppStandardThread
    prime.hpp
    div_range.hpp
    cpp_standard_thread.cpp
)

target_link_libraries(CppStandardThread
    PRIVATE Threads::Threads
)

if(MSVC)
    # Windows API thread
    add_executable(WinApiThread
        prime.hpp
        div_range.hpp
        win_api_thread.cpp
    )
else()
    # # Unix pthread
    # add_executable(PThread
    #     prime.hpp
    #     div_range.hpp
    # )
endif()