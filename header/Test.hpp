#ifndef __TEST__HPP__
#define __TEST__HPP__
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "Word.hpp"
#include "Exec.hpp"
#include "Arg.hpp"

using namespace std;

class Test: public Word {
    protected:
        string word;
    
    private:
        vector<Word*> sequence;

    public:
        Test() {};
	Test(vector<Word*> sequence){this->sequence = sequence;};
	std::string get_word(){return this->word;};
	void set_word(Word* word){ //clears the current sequence and appends the word in the argument
	    this->sequence.clear();
	    sequence.push_back(word);
	}
	int execute(){ //returns 0 if successful. returns 1 if failed
		struct stat stat_buf; //buf for the stat() 
		char *argv[sequence.size() + 1]; //create a char-pointer array
		for (int i = 0; i < sequence.size(); i++){
			argv[i] = const_cast<char*>(sequence.at(i)->get_word().c_str()); //move the pointers over. 
		}
		argv[sequence.size()] = NULL; //terminate with NULL
		switch(argv[1]){ //check the flag for the test command and execute the correct case that matches
		    case 'e': //checks if file/dir exits
			stat(argv[0], &stat_buf);
			if(stat_buf.st_mode == ){ //the mode gives the output. need to match using the macros 
			
			}
		    case 'f': //checks if it is a regular file
			stat(argv[0], &stat_buf);
			if(stat_buf.st_mode == ){

			}
		    case 'd': //checks if it is a directory
			stat(argv[0], &stat_buf);
			if(stat_buf.st_mode == ) {

			}
		}
	}
	void add_word(Word* new_word){
		if (word.size() !=0 ){
		    this->word.append(" ");
		}
		//update the word string of the command object
		this->word.append(new_word->get_word());
		//push the word object into the sequence
		this->sequence.push_back(new_word);
	}
        
};
#endif //__TEST_HPP__
