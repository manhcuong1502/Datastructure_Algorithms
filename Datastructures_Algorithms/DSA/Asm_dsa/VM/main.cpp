#include "VM.h"

using namespace std;

void test(string filename) {
    VM *vm = new VM();
    try {
        vm->run(filename);
    }
    catch (exception& e) {
        cout << e.what();
    }
    delete vm;
}

int main()
{
    char file[] = "prog*.txt";
    for (int i = 0; i < 10; i++) {
        file[4] = char(i + '0');
        test(file);
        cout << endl;
    }
    test("prog10.txt");
}