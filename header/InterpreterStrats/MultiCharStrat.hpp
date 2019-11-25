#ifndef __MULTICHARSTRAT_HPP__
#define __MULTICHARSTRAT_HPP__


#include "../Interpreter.hpp"
#include "InterpreterStrat.hpp"

#include <string>
#include <vector>
#include <iostream>

class MultiCharStrat : public InterpreterStrat{

        private:
                bool status;
		std::string character;
        public:
		MultiCharStrat(std::string characters){this->character = characters;}
                virtual std::vector<int> interpret(std::string token){
			//calls pause to the interpreter every time the character matches, sets status to true if it's a 100% match.
			std::vector<int> calls;
			//IF POTENTIALLY THE CHARACTER(S)
			if (token.size() == character.size()){
			    if (token == character){
				this->set_status(true);
				calls.push_back(3); //disable pause
			    } else {
				//cout << "\"" << token << "\" != " << character << std::endl;
			    	calls.push_back(3); //disable pause
			    }
			} else {
			    if (token == character.substr(0,token.size())){
				calls.push_back(8);
				calls.push_back(2); //enable pause
			    } else {
				//cout << "\"" << token << "\" != " << character.substr(0,token.size()) << std::endl;
				//TODO: this overlapping pause is preventing compatibility between multiple multicharstrats. fix this.
				
				//calls.push_back(3); //disable pause
			    }
			}
			
			return calls;
		}
                virtual std::vector<int> execute(){;} //this is decorated by character-specific strats, so we just leave this empty
                virtual bool get_status() {return this->status;}
		virtual bool set_status(bool value) {this->status = value;}
		virtual std::string get_character() {return this->character;}
};

#endif

