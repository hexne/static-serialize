module;
#include <meta>
export module serialize;
import std;


consteval auto get() {
    return std::meta::access_context::unchecked();
}




export
template <typename T>
void serialize() {
    constexpr auto info = ^^T;
    std::println("{}", std::meta::display_string_of(info));
}
