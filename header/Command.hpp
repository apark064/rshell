#ifndef __COMMAND__HPP__
#define __COMMAND__HPP__
#include <iostream>
#include <string>
#include <vector>

#include "Word.hpp"
#include "Exec.hpp"
#include "Arg.hpp"

class Command: public Word {
    protected:
        std::string word;
    
    private:
        std::vector<Word*> sequence;

    public:
        Command() {};
	void execute(){};
        
};
#endif //__COMMAND_HPP__
