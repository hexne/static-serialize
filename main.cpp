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

    void set(std::string val) {
        this->val = std::move(val);
    }
    std::string get() {
        return val;
    }
private:
    std::string val;
};



int main() {

    if (!std::filesystem::exists("data.bin")) {
        std::fstream file("data.bin", std::ios::out | std::ios::binary);
        Drived drived;
        drived.id = 1;
        drived.key = 1;
        drived.set("hello");
        static_serialize::serialize_impl(drived, file);
        std::println("serialize res: id={}, key={}, val={}", drived.id, drived.key, drived.get());
    }
    else {
        std::fstream file("data.bin", std::ios::in | std::ios::binary);
        Drived drived;
        drived.id = 2;
        drived.key = 2;
        drived.set("world");
        static_serialize::reserialize_impl(drived, file);
        std::println("serialize res: id={}, key={}, val={}", drived.id, drived.key, drived.get());
    }

    return 0;
}