module;
#include <meta>
#include <stdexcept>
#include <utility>
export module serialize;
import std;
#define NAMESPACE_BEGIN namespace static_serialize {
#define NAMESPACE_END }


export
NAMESPACE_BEGIN // namespace serialize

// 控制序列化格式
enum class serialize_flag {
    none, ignore
};

/**
 * @brief : 判断是否支持序列化和反序列化
 */
template <typename T>
concept have_custom_serialize = requires(T t) {
        t.serialize(std::declval<std::fstream&>());    // 有无成员函数
    }
    || requires(T t) {
        // 不使用作用域解析运算符，让编译器通过ADL查找
        serialize(std::declval<T&>(), std::declval<std::fstream&>());
    };

template<typename T>
concept have_custom_reserialize = requires(T t) {
        t.reserialize(std::declval<std::fstream&>());    // 有无成员函数
    }
    || requires(T t) {
        // 不使用作用域解析运算符，让编译器通过ADL查找
        reserialize(std::declval<T&>(), std::declval<std::fstream&>());
    };

    
template <typename T> 
consteval bool need_call_custom_operator() {
    return have_custom_serialize<T> && have_custom_reserialize<T>;
}



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



template <std::meta::info info>
consteval serialize_flag get_serialize_flag() {
    constexpr auto annotations = std::define_static_array(
        std::meta::annotations_of(info, ^^serialize_flag)
    );

    if constexpr (annotations.empty()) 
        return serialize_flag::none;
    else if (annotations.size() == 1) 
        return std::meta::extract<serialize_flag>(annotations.front());
    else 
        throw std::runtime_error("SerializeFlag 参数过多");
}


template <typename T>
void serialize(T obj, std::fstream &file) requires std::is_fundamental_v<T> {
    file.write(reinterpret_cast<const char*>(&obj), sizeof(T));
}

template <typename T>
void reserialize(T &obj, std::fstream &file) requires std::is_fundamental_v<T> {
    file.read(reinterpret_cast<char*>(&obj), sizeof(T));
}



enum class op_flag {
    serialize,
    reserialize
};

template <typename T>
void serialize_impl(T &obj, std::fstream &file) {

    // 对于基础类型
    if constexpr (std::is_fundamental_v<T>) {
        serialize(obj, file);
    }
    // @TODO 对于其他支持类型
    // 对于有自定义序列化和反序列化的类型
    else if constexpr (need_call_custom_operator<T>()) {
        if constexpr ( requires { obj.serialize(file); } )
            obj.serialize(file);
        else 
            serialize(obj, file);
    }
    // 对于其他类型
    else {
        constexpr auto members = std::define_static_array(
            std::meta::nonstatic_data_members_of(^^T, uncheck()));
        template for(constexpr auto info : members) {
            if constexpr (!is_data_member(info)) 
                continue;
            if constexpr (get_serialize_flag<info>() == serialize_flag::ignore)
                continue;
            serialize_impl<decltype(obj.[:info:])>(obj.[:info:], file);
        }

        // 静态数据成员
        constexpr auto static_members = std::define_static_array(
            std::meta::static_data_members_of(^^T, uncheck()));
        template for(constexpr auto info : static_members) {
            if constexpr (!is_data_member(info)) 
                continue;
            if constexpr (get_serialize_flag<info>() == serialize_flag::ignore)
                continue;
            serialize_impl<decltype(obj.[:info:])>(obj.[:info:], file);
        }
    }
}

template <typename T>
void reserialize_impl(T &obj, std::fstream &file) {

    // 对于基础类型
    if constexpr (std::is_fundamental_v<T>) {
        reserialize(obj, file);
    }
    // @TODO 对于其他支持类型
    // 对于有自定义序列化和反序列化的类型
    else if constexpr (need_call_custom_operator<T>()) {
        if constexpr ( requires { obj.reserialize(file); } )
            obj.reserialize(file);
        else 
            reserialize(obj, file);
    }
    // 对于其他类型
    else {
        constexpr auto members = std::define_static_array(
            std::meta::nonstatic_data_members_of(^^T, uncheck()));
        template for(constexpr auto info : members) {
            if constexpr (!is_data_member(info)) 
                continue;

            if constexpr (get_serialize_flag<info>() == serialize_flag::ignore)
                continue;
            
            reserialize_impl<decltype(obj.[:info:])>(obj.[:info:], file);
        }


        constexpr auto static_members = std::define_static_array(
            std::meta::static_data_members_of(^^T, uncheck()));
        template for(constexpr auto info : static_members) {
            if constexpr (get_serialize_flag<info>() == serialize_flag::ignore)
                continue;
            std::println("{} 反序列化", std::meta::display_string_of(info));
            reserialize_impl<decltype(obj.[:info:])>(obj.[:info:], file);
        }
    }
}

NAMESPACE_END   // namespace serialize end