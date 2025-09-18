import std;
import serialize;


struct Base {
    int key;
    std::string val;
};


int main() {

    serialize<Base>();


    return 0;
}