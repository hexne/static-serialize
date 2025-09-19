module;
#include <meta>
#include <stdexcept>
export module serialize;
import std;

template<typename T>
concept have_member_serialize = requires(T t, std::fstream &file) {
    t.serialize(file);
};

template<typename T>
concept have_global_serialize = requires(T t, std::fstream &file) {
    serialize(t, file);
};

template<typename T>
concept have_member_reserialize = requires(T t, std::fstream &file) {
    t.reserialize(file);
};

template<typename T>
concept have_global_reserialize = requires(T t, std::fstream &file) {
    reserialize(t, file);
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

export enum class SerializeFlag {
    none, ignore, bin, json
};

template <std::meta::info info>
consteval SerializeFlag get_serialize_flag() {
    constexpr auto annotations = std::define_static_array(
        std::meta::annotations_of(info, ^^SerializeFlag)
    );

    if constexpr (annotations.empty()) 
        return SerializeFlag::none;
    else if (annotations.size() == 1) 
        return std::meta::extract<SerializeFlag>(annotations.front());
    else 
        throw std::runtime_error("SerializeFlag 参数过多");
}


export
template <typename T>
void serialize(T obj, std::fstream &file) {
    if constexpr (support_serialize<T>) {
        if constexpr (have_member_serialize<T>)
            obj.serialize(file);
        else 
            serialize(obj, file);
    }
    else {
        constexpr auto members = get_members<T>();

        template for(constexpr auto info : members) {
            if constexpr (is_data_member(info)) {
                switch(get_serialize_flag<info>()) {
                    case SerializeFlag::ignore:
                        break;
                    case SerializeFlag::none:
                        [[fallthrough]];
                    default:
                        std::println("{}", std::meta::display_string_of(info));
                        break;
                }
            }
        }


    }
}