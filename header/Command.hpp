#ifndef __COMMAND__HPP__
#define __COMMAND__HPP__
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

#include "Word.hpp"
#include "Exec.hpp"
#include "Arg.hpp"

using namespace std;

class Command: public Word {
    protected:
        string word;
    
    private:
        vector<Word*> sequence;

    public:
        Command() {};
	Command(vector<Word*> sequence){this->sequence = sequence;};
	std::string get_word(){return this->word;};
	void set_word(Word* word){ //clears the current sequence and appends the word in the argument
	    this->sequence.clear();
	    sequence.push_back(word);
	}
	int execute(){ //returns 0 if successful. returns 1 if failed
		pid_t pid = fork();
		int status;
		if (pid == 0){ //if child process
			char *argv[sequence.size() + 1]; //create a char-pointer array
			for (int i = 0; i < sequence.size(); i++){
				argv[i] = const_cast<char*>(sequence.at(i)->get_word().c_str()); //move the pointers over. 
			}
			argv[sequence.size()] = NULL; //terminate with NULL
		    	execvp(argv[0],argv);
		} else {
			if (waitpid(pid,&status,0) > 0){;} //wait until child process is done
			if (WIFEXITED(status) && !WEXITSTATUS(status)){ //if child process successfully ended
				return 0;	
			} else {
				//cout << "failed" << endl;
				return 1;
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
#endif //__COMMAND_HPP__
