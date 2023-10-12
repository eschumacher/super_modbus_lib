
#include "mb_hello.hpp"
#include "mb_register.hpp"
#include <iostream>

using std::cout;
using std::endl;

class Test {
public:
    Test() {
        cout << "Constructor" << endl;
    }

    Test(const Test& t) {
        cout << "Copy Constructor" << endl;
    }

    Test(Test&& t) {
        cout << "Move Constructor" << endl;
    }

    Test& operator=(const Test& t) {
        cout << "Copy Assignment" << endl;

        return *this;
    }

    Test& operator=(Test&& t) {
        cout << "Move Assignment" << endl;

        return *this;
    }

    ~Test() {
        cout << "Destructor" << endl;
    }

private:
};

void testfn(const Test& t) {
    cout << "testfn start" << endl;

    Test loc = t;

    cout << "testfn done" << endl;
}

int main() {
    std::cout << "Hello, world!" << std::endl;

    hello();

    MBRegister<double> regf{};
    MBRegister regi{true};

    regf.print();
    regf.print_type();
    regi.print();
    regi.print_type();

    cout << "1" << endl;

    Test test;

    cout << "2" << endl;

    test = Test{};  // move

    cout << "3" << endl;

    Test test2 = test;  // copy

    cout << "4" << endl;

    test2 = {};  // move

    cout << "5" << endl;

    test = test2;

    cout << "6" << endl;

    test2 = std::move(test);

    cout << "7" << endl;

    testfn({});

    return 0;
}