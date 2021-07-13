/*Win32 thread のためのwrapper*/

#include <Windows.h>
#include <tuple>

#include <prime.hpp>

using thread_io = std::tuple<int, int, int>; // lo, hi, result

DWORD WINAPI thread_entry(LPVOID arg) {
    thread_io &io = *static_cast<thread_io *>(arg);
    get<2>(io) = count_prime(get<0>(io), get<1>(io));
    return 0;
}