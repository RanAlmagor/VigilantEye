#include <iostream>

int main() {
    // Check the standard version macro
    if (__cplusplus == 202002L) std::cout << "Running C++20" << std::endl;
    else if (__cplusplus == 201703L) std::cout << "Running C++17" << std::endl;
    else if (__cplusplus == 201402L) std::cout << "Running C++14" << std::endl;
    else if (__cplusplus == 201103L) std::cout << "Running C++11" << std::endl;
    else if (__cplusplus == 199711L) std::cout << "Running C++98" << std::endl;
    else std::cout << "Unknown Standard: " << __cplusplus << std::endl;

    return 0;
}