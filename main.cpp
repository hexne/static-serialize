#include <type_traits>
import std;
import serialize;


struct Base {
    [[=SerializeFlag::ignore]]
    int key = 10;
    long long val = 20;
    inline static float fval = 30.f;

    void serialize(std::fstream &file) {
        std::println("call serialize");
    }
    // void reserialize(std::fstream &file) {
    //     std::println("call reserialize");
    // }

    static void func() {

    }

};

template <typename ...Args>
void check() {
    static_assert((std::is_fundamental_v<Args> && ...));
}

int main() {

    std::fstream file;
    Base base;
    if (std::filesystem::exists("./bin")) {
        file.open("./bin", std::ios::binary | std::ios::in | std::ios::out);
        reserialize(base, file);
        std::println("{}", base.val);
    }
    else {
        file.open("./bin", std::ios::binary | std::ios::out);
        serialize(base, file);
    }
    return 0;
}