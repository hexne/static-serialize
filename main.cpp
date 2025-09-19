import std;
import serialize;


struct Base {
    [[=SerializeFlag::ignore]]
    int key = 10;
    std::string str = "20";
    inline static float fval = 30.f;

    void serialize() {
        std::println("call serialize");
    }
    // void reserialize() {
    //     std::println("call reserialize");
    // }

    static void func() {

    }

};


int main() {

    std::fstream file("./bin");
    Base base;
    serialize<Base>(base, file);


    return 0;
}