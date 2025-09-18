import std;
import serialize;


struct Base {
    int key = 10;
    std::string val = "20";

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