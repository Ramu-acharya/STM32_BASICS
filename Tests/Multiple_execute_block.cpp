#include <iostream>
#include <windows.h>
using namespace std;


#define WINDOWS

#ifdef WINDOWS
void print_Message(const std::string& message) {
    std::cout << "[Windows Log] " << message << std::endl;  // Green heavy check mark
}
#endif

#ifdef LINUX
void print_Message(const std::string& message) {
    std::cout << "[Linux Log] " << message << std::endl;
}
#endif

#ifdef UNIX
void print_Message(const std::string& message) {
    std::cout << "[Unix Log] " << message << std::endl;
}
#endif

int main(){
    print_Message("Program started successfully!");
    return 0;
}