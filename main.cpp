// import std;
#include <string>
#include <print>
#include <filesystem>
#include <fstream>
#include "serialize.cppm"


void print(auto vec) {
    std::print("{{");
    for(const auto &val : vec)
        std::print("{} ", val);
    std::print("}}");

}


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
    std::array<int, 5> vec;
private:
    std::string val;
};



int main() {

    if (!std::filesystem::exists("data.bin")) {
        std::fstream file("data.bin", std::ios::out | std::ios::binary);
        Drived drived;
        drived.id = 1;
        drived.key = 1;
        for (int i = 0;i < drived.vec.size(); ++i) 
            drived.vec[i] = i + 1;
        drived.set("hello");
        static_serialize::serialize(drived, file);
        std::println("serialize res: id={}, key={}, val={}", drived.id, drived.key, drived.get());
        print(drived.vec);
    }
    else {
        std::fstream file("data.bin", std::ios::in | std::ios::binary);
        Drived drived;
        drived.id = 2;
        drived.key = 2;
        for (int i = 0;i < drived.vec.size(); ++i) 
            drived.vec[i] = 5 - i;
        drived.set("world");
        static_serialize::deserialize(drived, file);
        std::println("deserialize res: id={}, key={}, val={}", drived.id, drived.key, drived.get());
        print(drived.vec);
    }

    return 0;
}