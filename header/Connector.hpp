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
        
};
#endif //__CONNECTOR__HPP__
