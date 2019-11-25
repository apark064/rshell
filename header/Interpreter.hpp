/* Note: This code could definitely be improved. There are a lot of "hard-coded" cases in here that could be replaced with more re-usable code.
 * 	 Also, theres some conditional statements (mainly in the double-character cases) that could be less redundant.
 * */

#ifndef __INTERPRETER_HPP__
#define __INTERPRETER_HPP__

#include <iostream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>

#include "Word.hpp"
#include "Command.hpp"
#include "Exec.hpp"
#include "Arg.hpp"
#include "Connector.hpp"

#include "InterpreterStrats/InterpreterStrat.hpp"

#include "InterpreterStrats/SingleCharStrat.hpp"
#include "InterpreterStrats/Comment.hpp"
#include "InterpreterStrats/Semicolon.hpp"

#include "InterpreterStrats/MultiCharStrat.hpp"
#include "InterpreterStrats/Doublepipe.hpp"
#include "InterpreterStrats/Doubleamp.hpp"

#include "InterpreterStrats/SpanStrat.hpp"
#include "InterpreterStrats/Quotes.hpp"

using namespace std;

class Interpreter {
    private:
        std::vector<Word*> word_list; //will only contain Command Objects and Connector Objects

	std::vector<int> calls;
	bool ignore = false; //if true, don't even interpret input
	bool pause = false; //if true, assumes current input is a part of previous input
	bool tokenReset = false; //if true, resets token interpretation. (used for new commands)
	int wordCount = 0; //the number of words interpreted. resets when token is resetted
	bool inAction = false; //if true,the interpreter is mid-interpretation (ex. n-char). switches to false at the end of each loop

	//strategies
	std::vector<InterpreterStrat*> strats = { //remember to include these characters in the char seperator
	    new Comment(new SingleCharStrat("#")),
	    new Semicolon(new SingleCharStrat(";")),
	    new Doublepipe(new MultiCharStrat("||")),
	    new Doubleamp(new MultiCharStrat("&&")),
	    new Quotes(new SpanStrat("\"")),
	    new Quotes(new SpanStrat("()")) //this is a test
	};


    public:
        Interpreter() {};
        Interpreter(std::vector<Word*> word_list) {this->word_list = word_list;};
        
	void InterpreterCall(unsigned index){
	    switch (index){
	    case 0: //0 = set ignore to true
		this->ignore = true;
		break;
	    case 1: //1 = set ignore to false
		this->ignore = false;
		break;
	    case 2: //2 = set pause to true. used for both N-char and Span
		this->pause = true;
		break;
	    case 3: //3 = set pause to false. used for both N-char and Span
		this->pause = false;
		break;
	    case 4: //4 = turn on tokenReset
		this->tokenReset = true;
		break;
	    case 5: //5 = turn off token reset
		this->tokenReset = false;
		break;
	    case 6: //6 = increment wordCount
		this->wordCount++;
		break;
	    case 7: //7 = decrement wordCount
		this->wordCount--;
		break;
	    case 8: //8 = in Action. turns off at the end of each token loop
		this->inAction = true;
	    default:
		break;
	    }
	}
	
	void makecalls(){ //helper function that makes all interpreter calls in calls vector
	    for (auto call = this->calls.begin(); call != this->calls.end(); call++){
	    	InterpreterCall(*call);
	    }
	    this->calls.clear();
	}
	

	int read_line(){ //returns 0 if not exiting. returns 1 if exiting
            std::string line;
	    std::cout << "$ ";
            std::getline(std::cin, line);
	    line.append(" "); //add a space at the end of the input so the interpreter catches the last token
            boost::char_separator<char> delim("", "|&;#\" ()"); //if the interpreter encounters any of the characters in the 2nd arg, it tokenizes the character individually
            boost::tokenizer<boost::char_separator<char>> tokens(line,delim); //TOKENINZE USER INPUT
		
		
	    //prepatory vars
	    std::string prev_token;
	    Command* command = new Command();

	    //INTERPRET TOKENS
	    for(auto it = tokens.begin(); it != tokens.end(); it++){   
		std::string token = *it;
		if (pause == true){ //add previous token if pause is on.
			//std::cout << "pause = true; appending \"" << prev_token << "\" to " << token << std::endl;
			token = prev_token.append(token);
		} else {
		    cout << "pause is off" << endl;
		}
		cout << "token: " << token <<endl;
		cout << "1pause: " << this->pause << endl;
		//go through various interpretation strategies
		for (auto strit = strats.begin(); strit != strats.end(); strit++){
		    if (this->inAction == false){
		        calls = (*strit)->interpret(token);
			this->makecalls();
		    }
		}
		cout << "2pause: " << this->pause << endl;
		//make interpreter calls according to interpretations
		for (auto strit = strats.begin(); strit != strats.end(); strit++){
		    if (this->ignore == false){
			if ((*strit)->get_status() == true){
			    std::cout << (*strit)->get_character() << " is active." << std::endl;
			    calls  = (*strit)->execute();
			    this->makecalls();
			}
	   	    }
		}	
		cout << "3pause: " << this->pause << endl;
		//IF NOT IGNORING TOKENS
		if (this->ignore == false){
		    if (tokenReset == false){
			if (pause == false && inAction == false){
		    	    if (token != " "){ //ignore space tokens
		    	        if (wordCount == 0){ //APPENED A EXEC
			            command->add_word(new Exec(token));
				    std::cout << "\"" << token << "\" was appended as an EXEC. Wordcount: " << this->wordCount <<  std::endl;
			        } else { //APPEND AN ARGa
				    command->add_word(new Arg(token));
				    std::cout << "\"" << token << "\" was appended as an ARG. Wordcount: " << this->wordCount << std::endl;
				    }
			        this->wordCount++;    
			        //cout << "New Wordcount: " << this->wordCount << endl;
				prev_token = token;
			    } else {
			    	//`std::cout << "pause is false but space detected" << std::endl;
			    }
			} else {
			    prev_token = token;
			    std::cout << "pause is true. new token is \"" << prev_token << "\"" << std::endl;
			}
		    } 
		    //RESET TOKENS IF NECESSARY
		    else {
			//append the built command we have made and instantiate a new command object to append to
			word_list.push_back(command);
			command = new Command();
			
			//append our connector
			this->word_list.push_back(new Connector(token));
			std::cout << "\"" << token << "\" was appended as a CONNECTOR" << std::endl;
			
			//reset
		        prev_token = "";
		        this->tokenReset = false;
		        this->wordCount = 0;
		    }
		}
		this->inAction = false;
            }
	    //EOL
	
	    word_list.push_back(command); //push the last command in
	    this->ignore = false;
	    this->wordCount = 0; //reset wordCount since we are at the end of the line
	    //EXECUTE COMMANDS
	    int prevExecute = 0; //updates based on the previous execute()
	    bool ignore2 = false; //if true, ignores the next command
	    for (auto it = this->word_list.begin(); it != this->word_list.end(); it++){
		
		for (auto strit = strats.begin(); strit != strats.end(); strit++){ //check for conditional running for our strategies
		    if ((*strit)->get_character() == (*it)->get_word()){ //if our word matches any of our interpretation strategies (connectors)
			//do stuff based on the strategy
		    }
		}
		
		if ((*it)->get_word() == "exit"){ //exit command
		    return 1;
		}
		
		if (ignore2 == false){ 
		    prevExecute = dynamic_cast<Command*>(*it)->execute();
	    	    if (prevExecute == -1){
		        std::cout << "Invalid Command" << std::endl;
		    }
		} else {
		    it++; //skip this word and move to the next
		    ignore2 = false; 
		    prevExecute = 0; //reset prevExecute
		}

	    }
	    word_list.clear();
	    for (auto strit = strats.begin(); strit != strats.end(); strit++){(*strit)->set_status(false);} //set all status of strats to false since we are done with the line
	    return 0;
        };

};

#endif
