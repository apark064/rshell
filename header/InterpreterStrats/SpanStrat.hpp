#ifndef __SPANSTRAT_HPP__
#define __SPANSTRAT_HPP__


#include "../Interpreter.hpp"
#include "InterpreterStrat.hpp"

#include <string>
#include <vector>

class SpanStrat : public InterpreterStrat{

        private:
                bool status;
		std::string character;
		std::vector<std::string> captures; //a vector of strings that contains our "captured strings". the size of the vector is  nesting. (ex captures.at(0) = outer span, captures.at(1) = inner span
        public:
		SpanStrat(std::string characterPair){
		    if (characterPair.size() == 1){ characterPair.append(characterPair);} //double our character to match our format
		    this->character = characterPair;
		}
                std::vector<int> interpret(std::string token){
			std::vector<int> calls;
			
			if (token == string(1,character.at(0))){ //BEGINNING CHARACTER
			    captures.push_back(""); //append an empty string to the captures vector (signifies adding a layer)
			    this->set_status(true); //activate
			    calls.push_back(8); //enable inAction so no other strats get in the way
			    calls.push_back(2); //enable pause
			    std::cout << "Span opened. captures size is now: " << captures.size() <<std::endl;
				
			} else if (string(1,token.at(token.size()-1)) == string(1,character.at(1))){ // ENDING CHARACTER
			    captures.at(captures.size()-1).append(string(1,token.at(token.size()-1)));
			    std::string prevSpan = captures.at(captures.size()-1);;
			    std::cout << "Span closed. Span held \"" << prevSpan << "\"" << std::endl;
			    captures.pop_back();
			    if (captures.size() != 0){
			    	captures.at(captures.size()-1).append(prevSpan);
			    } else {
			    	calls.push_back(3); //disable pauses
			    	this->set_status(false);
			    }
			
			} else {
			    if (this->get_status() == true){ //if capturing.
				//std::cout << "\"" << token << "\" != \"" << string(1,character.at(1)) << "\"" << std::endl;
				//std::cout << "Span is still open. Appending: \"" << token << "\"" << std::endl;
				captures.at(captures.size() - 1).append(token); 
				calls.push_back(8); //enable inAction
			    }
			}
			return calls;
		}
		
		//spanstrat decorators MUST call their base's exectute() at the beginning of the call and save the output to be returned at the end of the call.
                std::vector<int> execute(){
			//call something to delete the ends of the span. if the the span is completed.
			
			
			
			return {-1};
		}
                bool get_status() {return this->status;}
		bool set_status(bool value) {this->status = value;}
		std::string get_character() {return this->character;}
};

#endif

