// import std;
#include <string>
#include <print>
#include <filesystem>
#include <fstream>
#include "serialize.cppm"

struct Base {
    int id{};
    static void serialize(std::fstream &file) {
        std::println("call serialize");
    }
};

struct Drived : Base {
    int key{};
    std::string val;
};



int main() {

    if (!std::filesystem::exists("data.bin")) {
        std::fstream file("data.bin", std::ios::out | std::ios::binary);
        Drived drived { .key = 1, .val = "hello" };
        drived.id = 1;
        static_serialize::serialize_impl(drived, file);
        std::println("serialize res: id={}, key={}, val={}", drived.id, drived.key, drived.val);
    }
    else {
        std::fstream file("data.bin", std::ios::in | std::ios::binary);
        Drived drived { .key = 2, .val = "world" };
        drived.id = 2;
        static_serialize::reserialize_impl(drived, file);
        std::println("reserialize res: id={}, key={}, val={}", drived.id, drived.key, drived.val);
    }

    return 0;
}