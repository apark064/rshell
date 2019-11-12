#ifndef __WORD__HPP__
#define __WORD__HPP__
#include <iostream>
#include <string>

class Word {
    protected:
        std::string word;

    public:
        Word() {};
	Word(std::string word);
        virtual std::string get_word() = 0;
};
#endif //__WORD__HPP__
