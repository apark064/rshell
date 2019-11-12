#ifndef __ARG__HPP__
#define __ARG__HPP__
#include <iostream>
#include <string>

#include "Word.hpp"

class Arg : public Word {
    protected:
        std::string word;

    public:
        Arg() {};
	Arg(std::string word){this->word = word;};
        
};
#endif //__ARG__HPP__
