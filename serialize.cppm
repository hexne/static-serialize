module;
#include <meta>
export module serialize;
import std;

template<typename T>
concept have_member_serialize = requires(T t) {
    t.serialize();
};

template<typename T>
concept have_global_serialize = requires(T t) {
    serialize(t);
};

template<typename T>
concept have_member_reserialize = requires(T t) {
    t.reserialize();
};

template<typename T>
concept have_global_reserialize = requires(T t) {
    reserialize(t);
};





template <typename T>
concept have_serialize = have_global_serialize<T> || have_member_serialize<T>;

template <typename T>
concept have_reserialize = have_global_reserialize<T> || have_member_reserialize<T>;

template <typename T>
concept support_serialize = have_serialize<T> && have_reserialize<T>;

consteval auto uncheck() {
    return std::meta::access_context::unchecked();
}
template <typename T>
consteval auto get_members() {
    return std::define_static_array(
        std::meta::members_of(^^T, std::meta::access_context::unchecked())
    );
}

consteval bool is_data_member(std::meta::info info) {
    return std::meta::is_nonstatic_data_member(info)
        || (is_static_member(info) && is_variable(info));
}

export
template <typename T>
void serialize(T obj, std::fstream &) {
    if constexpr (support_serialize<T>) {
        if constexpr (have_member_serialize<T>)
            obj.serialize();
        else 
            serialize(obj);
    }
    else {
        constexpr auto members = get_members<T>();

        template for(constexpr auto info : members) {
            if constexpr (is_data_member(info)) {
                std::println("{}", std::meta::display_string_of(info));

            }
            else {
                std::println("else {}", std::meta::display_string_of(info));

            }
        }


    }
}