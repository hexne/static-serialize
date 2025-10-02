#pragma once

// module;
#include <stdexcept>
#include <utility>
#include <print>
#include <string>
#include <type_traits>
#include <meta>
// export module serialize;
// import std;
#define NAMESPACE_BEGIN namespace static_serialize {
#define NAMESPACE_END }


// export
NAMESPACE_BEGIN // namespace serialize

// 控制序列化格式
enum class serialize_flag {
    none, ignore
};

template <typename T>
struct BinarySerializer {  };

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

template <typename T>
concept have_binary_serializer = requires {
        BinarySerializer<T>::serialize(std::declval<T>(), std::declval<std::fstream&>());
        BinarySerializer<T>::reserialize(std::declval<T&>(), std::declval<std::fstream&>());
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

template <typename T>
constexpr bool have_built_in_support = std::is_fundamental_v<T>
                            || std::is_same_v<T, std::string>;

template <typename T>
struct have_pointer_type : std::false_type {};

template <typename T>
struct have_pointer_type<std::shared_ptr<T>> : std::true_type {};
template <typename T>
struct have_pointer_type<std::unique_ptr<T>> : std::true_type {};


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
    requires std::is_fundamental_v<T>
struct BinarySerializer<T> {
    static void serialize(T obj, std::fstream &file) {
        file.write(reinterpret_cast<const char*>(&obj), sizeof(T));
    }
    static void reserialize(T &obj, std::fstream &file) {
        file.read(reinterpret_cast<char*>(&obj), sizeof(T));
    }
};

template <>
struct BinarySerializer<std::string> {
    static void serialize(std::string& str, std::fstream& file) {
        size_t size = str.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        file.write(str.data(), size);
    }

    static void reserialize(std::string& str, std::fstream& file) {
        size_t size = 0;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));
        str.resize(size);
        file.read(str.data(), size);
    }
};


#define def(OP)\
template <typename T>\
void OP(T &obj, std::fstream &file) {\
    if constexpr (have_built_in_support<T> || have_binary_serializer<T>) {\
        BinarySerializer<T>::OP(obj, file);\
    }\
    else if constexpr (need_call_custom_operator<T>()) {\
        if constexpr ( requires { obj.OP(file); } )\
            obj.OP(file);\
        else \
            OP(obj, file);\
    }\
    else {\
        constexpr auto bases_info = std::define_static_array(\
            std::meta::bases_of(^^T, std::meta::access_context::unchecked())\
        );\
        template for (constexpr auto base_info : bases_info) {\
            using Base = typename [:std::meta::type_of(base_info):];\
            OP<Base>(static_cast<Base&>(obj), file);\
        }\
        constexpr auto members = std::define_static_array(\
            std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::unchecked()));\
        template for(constexpr auto info : members) {\
            if constexpr (get_serialize_flag<info>() == serialize_flag::ignore)\
                continue;\
            OP<decltype(obj.[:info:])>(obj.[:info:], file);\
        }\
        constexpr auto static_members = std::define_static_array(\
            std::meta::static_data_members_of(^^T, std::meta::access_context::unchecked()));\
        template for(constexpr auto info : static_members) {\
            if constexpr (get_serialize_flag<info>() == serialize_flag::ignore)\
                continue;\
            OP<decltype(obj.[:info:])>(obj.[:info:], file);\
        }\
    }\
}

def(serialize);
def(reserialize);
#undef def


NAMESPACE_END   // namespace serialize end