#ifndef __CONNECTOR__HPP__
#define __CONNECTOR__HPP__
#include <iostream>
#include <string>

#include "Word.hpp"

class Connector : public Word {
    protected:
        std::string word;

    public:
        Connector() {};
        Connector(std::string word){this->word = word;};
        std::string get_word(){return this->word;};
};
#endif //__CONNECTOR__HPP__
