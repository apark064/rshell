#ifndef __COMMAND__HPP__
#define __COMMAND__HPP__
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>

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
	void execute(){
		string line = "";
		for (auto it = this->sequence.begin(); it != this->sequence.end(); it++){
			line.append(it.word); //word is protected so it should be accessible
			line.append(" "); //add a space
		}
		line.erase(line.end()); //delete the trailing space at the end
		char* args[] = {"/bin/bash", "-c", line, NULL};
        	execvp(args[0], args);
	};
        
};
#endif //__COMMAND_HPP__
