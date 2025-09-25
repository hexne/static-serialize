module;
#include <meta>
#include <stdexcept>
export module serialize;
import std;

/**
 * @brief : 判断是否支持序列化和反序列化
 */
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
concept supported_type = have_serialize<T> && have_reserialize<T>;

consteval auto uncheck() {
    return std::meta::access_context::unchecked();
}

/**
 * @brief : 获取所有的成员
 */
template <typename T>
consteval auto get_members() {
    return std::define_static_array(
        std::meta::members_of(^^T, std::meta::access_context::unchecked())
    );
}

/**
 * @brief : 获取info下的所有数据成员
 */
consteval bool is_data_member(std::meta::info info) {
    return std::meta::is_nonstatic_data_member(info)
        || (is_static_member(info) && is_variable(info));
}

// 控制序列化格式
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



export template <typename T>
void serialize(T obj, std::fstream &file) {
    
    if constexpr (std::is_fundamental_v<T>) {
        file.write(reinterpret_cast<const char*>(&obj), sizeof(T));
        return;
    }
    else if constexpr (supported_type<T>) {
        if constexpr (have_member_serialize<T>)
            obj.serialize(file);
        else 
            serialize(obj, file);
    }
    // 生成序列化和反序列化
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

export template <typename T>
void reserialize(T &obj, std::fstream &file) {


    // 有特定的函数进行序列化和反序列化
    if constexpr (supported_type<T>) {
        if constexpr (have_member_reserialize<T>)
            obj.reserialize(file);
        else 
            reserialize(obj, file);
    }
    // 生成反序列化
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