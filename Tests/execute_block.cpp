//Loop executes if TEST_PROGRAM is defined
#include<iostream>
using namespace std;
#define TEST_PROGRAM//comment this line to disable the test program
int main() {
    int a = 0;
    #ifdef TEST_PROGRAM
    for (int i = 0; i < 5; i++) {
        a += i;
    }
    #endif
    cout <<"Value of a:\n"<<a << endl;
    return 0;
}
