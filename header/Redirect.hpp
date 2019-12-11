#ifndef __REDIRECT__HPP__
#define __REDIRECT__HPP__
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

//dup
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//rename
#include <stdio.h>

#include "Word.hpp"
#include "Exec.hpp"
#include "Arg.hpp"
#include "Command.hpp"

using namespace std;

class Redirect: public Command {
    protected:
        string word;
    
    private:
	Command* Base;
        vector<Word*> sequence;

    public:
        Redirect(Command* base) {this->Base = base;}
	std::string get_word(){return Base->get_word();}
	void set_word(Word* word){this->Base->set_word(word);}
	std::string execute(){ //returns "16 0" if successful. returns "16 1" if failed
		//std::cout << "Redirect!" << std::endl;
		this->sequence = Base->get_sequence(); //get sequence from base command
		
		//SET FILE TO BE NEW OUTPUT
		int stdFd = dup(1);
		close(1);
		int openFd = open("TEMP_REDIRECT",O_CREAT|O_RDWR|O_TRUNC,S_IRWXU); //create a new file called TEMP_REDIRECT
		int outFd = dup(openFd); //this is dup'd into our 1, which makes outFd our current output
		
		//RUN COMMAND AS NORMAL
		std::string result = this->Base->execute(); //call execute as normal. anything in sent to cout is sent to outFd.
		
		//SET STDOUT TO OUTPUT AGAIN
		close(openFd); //close the opened file
		close(outFd); //close our output fd
	
		dup(stdFd); //dup our std back into output
		close(stdFd); //close our dup'd sd
		
		//std::cout << "Reached end of Redirect!" << std::endl;
		return result;

	}
	void add_word(Word* new_word){this->Base->add_word(new_word);}
        
};

class Redirect_Target : public Command{
    protected:
        string word;
    
    private:
	Command* Base;
        vector<Word*> sequence;

    public:
        Redirect_Target(Command* base) {this->Base = base;}
	std::string get_word(){return Base->get_word();}
	void set_word(Word* word){this->Base->set_word(word);}
	std::string execute(){ //returns "16 0" if successful. returns "16 1" if failed
		//std::cout << "Redirect Target!" <<std::endl;
		const char* FILEPATH = Base->get_word().c_str();
		
		//if FILEPATH is a real path, "set" the contents of FILEPATH to the contents to TEMP_REDIRECT. and then delete TEMP_REDIRECT
		int exists = access(FILEPATH,F_OK);
		if (exists == 0){ //delete FILEPATH, since it exists. we out here doing ghetto rewrites
		    remove(FILEPATH);
		}
		rename("TEMP_REDIRECT",FILEPATH);

		return "16 0";
	}
	void add_word(Word* new_word){this->Base->add_word(new_word);}
};
#endif //__REDIRECT_HPP__
