module;
#include <meta>
export module serialize;
import std;

export void func() {
    constexpr auto info = ^^int;
    std::println("{}", std::meta::display_string_of(info));
}