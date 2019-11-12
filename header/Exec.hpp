#ifndef __EXEC__HPP__
#define __EXEC__HPP__
#include <iostream>
#include <string>

#include "Word.hpp"

class Exec : public Word {
    protected:
        std::string word;

    public:
        Exec() {};
        Exec(std::string word){this->word = word;};
};
#endif //__EXEC__HPP__
