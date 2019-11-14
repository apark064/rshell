#include "../header/Interpreter.hpp"
#include "../header/Word.hpp"

#include <iostream>

int main() {
    
    std::cout << "RSHELL" << std::endl;
    Interpreter* shell = new Interpreter();
 
    while (shell->read_line() == 0){ ; }
    return 0;
}
