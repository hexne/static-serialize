import std;
import serialize;


struct Base {
    [[=static_serialize::serialize_flag::ignore]]
    int key = 10;
    long long val = 20;
    inline static float fval = 30.f;

    void serialize(std::fstream &file) {
        std::println("call serialize");
    }

};


int main() {

    if (std::filesystem::exists("data.bin")) {
        std::fstream file("data.bin", std::ios::in | std::ios::binary);
        Base base { .key = 2, .val = 2 };
        Base::fval = 2;
        static_serialize::reserialize_impl(base, file);
        std::println("key={}, val={}, fval={}", base.key, base.val, base.fval);
    }
    else {
        std::fstream file("data.bin", std::ios::out | std::ios::binary);
        Base base { .key = 1, .val = 1 };
        Base::fval = 1;
        static_serialize::serialize_impl(base, file);
        std::println("key={}, val={}, fval={}", base.key, base.val, base.fval);
    }


    return 0;
}