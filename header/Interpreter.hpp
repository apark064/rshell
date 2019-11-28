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
#include "InterpreterStrats/Paren.hpp"
#include "InterpreterStrats/Brackets.hpp"

using namespace std;

class Interpreter {
    private:
 	std::vector<std::string> calls; //holds interpreter calls. clears after they are all called
	bool HALT_INTERPRETATION = false; //if true, don't even interpret input
	bool APPEND_TO_PREVIOUS_TOKEN = false; //if true, assumes current input is a part of previous input
	bool END_OF_COMMAND = false; //if true, resets token interpretation. (used for new commands)
	int wordCount = 0; //the number of words interpreted. resets when token is resetted
	bool INTERPRETATION_IN_PROGRESS = false; //if true,the interpreter is mid-interpretation (ex. n-char). switches to false at the end of each loop
	std::string TOKEN_OVERRIDE; //if this string is not empty, the interpreter replaces the current token with the value of this string and clears the override.
	std::string STRATEGY_IN_PROGRESS; //if this string is not empty, the interpreter can only pay attention to the strategy in this string.
	
	int PREVIOUS_EXECUTE = -1; //the status of the previous command->execute(). used for nested commands
	std::vector<Word*>* CURRENT_WORD_LIST; //a pointer to the current word_list that the interpreter is working on. used for referencing the wordlist outside of the read_line function call	

	//strategies
	std::vector<InterpreterStrat*> strats = {
	    new Comment(new SingleCharStrat("#")),
	    new Semicolon(new SingleCharStrat(";")),
	    new Doublepipe(new MultiCharStrat("||")),
	    new Doubleamp(new MultiCharStrat("&&")),
	    new Quotes(new SpanStrat("\"")),
	    new Paren(new SpanStrat("()")),
	    new Brackets(new SpanStrat("[]"))
	};


    public:
        Interpreter() {};
        
	void InterpreterCall(std::string input){
	    //format: "XX string input"
	    //the first 2 characters are an interpreter code. this is followed by a whitespace
	    //all of the characters after the whitespace is string input that is used for some codes. 
	    int INTERPRETER_CODE = -1;
	    try{
	        INTERPRETER_CODE = std::stoi(input.substr(0,2),nullptr);
	    } catch(...){
	        cout << "Invalid Interpreter Code: '" << input.substr(0,2) << "'" << endl;
	   	return;
	    }
	    std::string STRING_INPUT;
	    if (input.size() > 2){
	    	STRING_INPUT = input.substr(3);
	    } else {
	        STRING_INPUT = "NO STRING INPUT";
	    }
	    //cout << "INT CODE: " << INTERPRETER_CODE << endl;
	    //cout << "STRING_INPUT: " << STRING_INPUT <<endl;
	   
	    switch (INTERPRETER_CODE){
	        case 0: this->HALT_INTERPRETATION = true; break; //0 = set ignore to true
	        case 1: this->HALT_INTERPRETATION = false; break; //1 = set ignore to false
	        case 2: this->APPEND_TO_PREVIOUS_TOKEN = true; break; //2 = set pause to true. used for both N-char and Span
	        case 3: this->APPEND_TO_PREVIOUS_TOKEN = false; break; //3 = set pause to false. used for both N-char and Span
	        case 4: this->END_OF_COMMAND = true; break; //4 = turn on tokenReset
	        case 5: this->END_OF_COMMAND = false; break; //5 = turn off token reset
	        case 6: this->wordCount++; break; //6 = increment wordCount
	        case 7: this->wordCount--; break; //7 = decrement wordCount
	        case 8: this->INTERPRETATION_IN_PROGRESS = true; break; //8 = in Action. turns off at the end of each token loop
		case 9: this->TOKEN_OVERRIDE = STRING_INPUT; break; //9 = set TOKEN_OVERRIDE to string input
		case 10: this->PREVIOUS_EXECUTE = temp_execute(STRING_INPUT); break; //10 = immediately run STRING_INPUT as a command.
		case 11: temp_insert(CURRENT_WORD_LIST, temp_read(STRING_INPUT)); break; //interprets STRING_INPUT and pushes all contents to to CURRENT_WORD_LIST
		case 12: this->STRATEGY_IN_PROGRESS = ""; break; //12 = clears STRATEGY_IN_PROGRESS
		case 13: cout << STRING_INPUT << endl; break; //13 = output STRING_INPUT
	        case 14: //set all vars to false
		    this->HALT_INTERPRETATION = false;
		    this->APPEND_TO_PREVIOUS_TOKEN = false;
		    this->END_OF_COMMAND = false;
		    this->INTERPRETATION_IN_PROGRESS = false;
		    this->STRATEGY_IN_PROGRESS = "";
		    break;
		default: break;
	    }
	}
	
	void temp_insert(std::vector<Word*>* a, std::vector<Word*> b){ //a helper function for temporary vector insertion since we can't pull this off in the switch case
		a->insert(a->end(),b.begin(),b.end());   
	}
	
	std::vector<Word*> temp_read(std::string input){ //helper function that creates an interpreter that reads input and returns a word vector
	    Interpreter* temp = new Interpreter;
	    std::vector<Word*> word_list = temp->read_line(input);
	    delete temp;
	    return word_list;
	}

	int temp_execute(std::string input){ //helper function that creates a temporary interpreter that reads and runs the input. returns the result
	    Interpreter* temp = new Interpreter;
	    //cout << "plugging in '" << input << "' into temporary interpreter." << endl;
	    int result = temp->execute(temp->read_line(input));    
	    delete temp;
	    return result;
	}
	
	void makecalls(){ //helper function that makes all interpreter calls in calls vector
	    for (auto call = this->calls.begin(); call != this->calls.end(); call++){
	    	InterpreterCall(*call);
	    }
	    this->calls.clear();
	}
	
	void debug(int layer, std::string m){ //a debug helper function that just prints a colored message with a certain amount of indentation`
	    for (int i = 0; i < layer; i++){std::cout << "    ";}
	    std::cout << m << std::endl;
	}
	
	std::vector<Word*> read_line(std::string line){ //returns a vector of Word Objects (only Command and Connector objects)
	    std::vector<Word*> word_list;
	    CURRENT_WORD_LIST = &word_list;
	    line.append(" "); //append space character so the last token runs.
	    
	    //take all of the characters in our strategy and put them into a char array as sperators characters for our tokenizer
	    std::string strategyChars = " ";
	    for (auto strit = strats.begin(); strit != strats.end(); strit++){
	        if (strategyChars.find((*strit)->get_character()) == std::string::npos){
		    strategyChars.append((*strit)->get_character());
		}
	    }	
            boost::char_separator<char> delim("", strategyChars.c_str()); //if the interpreter encounters any of the characters in the 2nd arg, it tokenizes the character individually
            boost::tokenizer<boost::char_separator<char>> tokens(line,delim); //TOKENINZE USER INPUT
		
	    //prepatory vars
	    std::string PREVIOUS_TOKEN;
	    Command* command = new Command();

	    //INTERPRET TOKENS
	    for(auto it = tokens.begin(); it != tokens.end(); it++){   
		std::string CURRENT_TOKEN = *it;
		if (this->APPEND_TO_PREVIOUS_TOKEN == true){ //add previous token if pause is on.
			//std::cout << "pause = true; appending \"" << PREVIOUS_TOKEN << "\" to " << CURRENT_TOKEN << std::endl;
			CURRENT_TOKEN = PREVIOUS_TOKEN.append(CURRENT_TOKEN);
		} else {
		    //debug(1,"APPEND_TO_PREVIOUS_TOKEN is off");
		}
		//debug(0, "token: " + CURRENT_TOKEN);
		//go through various interpretation strategies
		for (auto strit = strats.begin(); strit != strats.end(); strit++){
		    if (this->INTERPRETATION_IN_PROGRESS == false){
			if (this->STRATEGY_IN_PROGRESS == "" || this->STRATEGY_IN_PROGRESS == (*strit)->get_character()){
		            calls = (*strit)->interpret(CURRENT_TOKEN);
			    this->makecalls();
			    if (this->INTERPRETATION_IN_PROGRESS == true){ //if inAction was set to true from the current strategy
			        this->STRATEGY_IN_PROGRESS = (*strit)->get_character();
			    } 
			} else {
			    //debug(1, "STRATEGY_IN_PROGRESS != 0 or same strat");
			}
		    }
		}
		//make interpreter calls according to interpretations
		for (auto strit = strats.begin(); strit != strats.end(); strit++){
		    if (this->HALT_INTERPRETATION == false){
			if ((*strit)->get_status() == true){
			    //debug(2,(*strit)->get_character() + " status: " +(*strit)->get_status());
			    //debug(2, (*strit)->get_character() + " is executing.");
			    calls  = (*strit)->execute();
			    this->makecalls();
			}
	   	    }
		}
		//override current token if TOKEN_OVERRIDE isn't empty.
		if (TOKEN_OVERRIDE != ""){
		    CURRENT_TOKEN = TOKEN_OVERRIDE;
		    this->TOKEN_OVERRIDE = ""; //reset override
		}

		//IF NOT IGNORING TOKENS
		if (this->HALT_INTERPRETATION == false){
		    if (this->END_OF_COMMAND == false){
			if (this->APPEND_TO_PREVIOUS_TOKEN == false && this->INTERPRETATION_IN_PROGRESS == false){
		    	    if (CURRENT_TOKEN != " "){ //ignore space tokens
		    	        if (wordCount == 0){ //APPENED A EXEC
			            command->add_word(new Exec(CURRENT_TOKEN));
				    //debug(2,"\"" + CURRENT_TOKEN + "\" was appended as an EXEC. Wordcount: " + to_string(this->wordCount));
			        } else { //APPEND AN ARGa
				    command->add_word(new Arg(CURRENT_TOKEN));
				    //debug(2,"\"" + CURRENT_TOKEN + "\" was appended as an ARG. Wordcount: " + to_string(this->wordCount));
				    }
			        this->wordCount++;    
			        //cout << "New Wordcount: " << this->wordCount << endl;
				PREVIOUS_TOKEN = CURRENT_TOKEN;
			    } else {
			    	//`std::cout << "pause is false but space detected" << std::endl;
			    }
			} else {
			    PREVIOUS_TOKEN = CURRENT_TOKEN;
			    //debug(2, "APPEND_TO_PREVIOUS_TOKEN is true. new prev_token is \"" + prev_token + "\"");
			}
		    } 
		    //RESET TOKENS IF NECESSARY
		    else {
			//append the built command we have made and instantiate a new command object to append to
			word_list.push_back(command);
			command = new Command();
			
			//append our connector
			word_list.push_back(new Connector(CURRENT_TOKEN));
			//debug(2, "\"" + CURRENT_TOKEN + "\" was appended as a CONNECTOR");
			
			//reset
		        PREVIOUS_TOKEN = "";
		        this->END_OF_COMMAND = false;
		        this->wordCount = 0;
		    }
		}
		this->INTERPRETATION_IN_PROGRESS = false;
            }
	    //EOL
	    word_list.push_back(command); //push the last command in
	    
	    //check for invalid inputs (ex: incomplete spans)
	    for (auto strit = strats.begin(); strit != strats.end(); strit++){
	    	if ((*strit)->get_status() == true){
		    cout << "ERROR: Invalid Input. (Maybe an incomplete span?)" << endl;
		    word_list.clear();
		    this->InterpreterCall("14");
		}
		(*strit)->reset_vars();
		(*strit)->set_status(false);
	    }    
	
	    this->wordCount = 0; //reset wordCount since we are at the end of the line
	    return word_list;
	}

	int execute(std::vector<Word*> word_list){ //executes commands in the passed in vector of words objects.
	    //EXECUTE COMMANDS
	    bool IGNORE_NEXT_WORD = false; //if true, ignores the next command
	    bool AVOID_EXECUTE = false; //if true, avoids the execution of the command	    
	
	    //std::cout << "here is our wordlist: " << endl;
	    //for (unsigned i = 0; i < word_list.size(); i++){
	    //std::cout << "word " << i << ": '" << word_list.at(i)->get_word() << "'" << std::endl;
	    //}

	    for (auto it = word_list.begin(); it != word_list.end(); it++){
		//cout << "checking '" << (*it)->get_word() << "'" << endl;
		for (auto strit = strats.begin(); strit != strats.end(); strit++){ //check for conditional running for our strategies
		    if ((*strit)->get_character() == (*it)->get_word()){ //if our word matches any of our interpretation strategies (connectors)
		    	if ((*strit)->previous_must_be() != -1){ //-1 means the strategy doesn't need any previous output. by using !=, the strategy requires a certain output
			    if (PREVIOUS_EXECUTE != (*strit)->previous_must_be()){
			    	AVOID_EXECUTE = true;
				IGNORE_NEXT_WORD = true;
				//cout << "this should be ignoring" << endl;
			    } else {
				//std::cout << "PREVIOUS EXECUTE (" << PREVIOUS_EXECUTE << ") == " << (*strit)->get_character() << "'s " << (*strit)->previous_must_be() << " so we will let '" << (*it)->get_word() << "' to execute." << std::endl;
			    }
		
			}
		    }
		}
		
		//what we essentially have here are custom commands for things.

		if ((*it)->get_word() == "exit"){ //exit command
		    return 2; //2 is now our new exit command
		} else if ((*it)->get_word() == "test"){ //test command
		    //need to add.
		}
		} else if ((*it)->get_word() == ""){ //blank command (if plugging in only a span
		    AVOID_EXECUTE = true;
		    IGNORE_NEXT_WORD = false; //acknowledge next word
		} else if ((*it)->get_word() == "(True)"){ //logic override
		    PREVIOUS_EXECUTE = 0;
		    AVOID_EXECUTE = true;
		} else if ((*it)->get_word() == "(False)"){ //logic override
		    PREVIOUS_EXECUTE = 1;
		    AVOID_EXECUTE = true;
		} else if ((*it)->get_word() == "(Carry Over)"){ //this just passes on the same PREVIOUS_EXECUTE.
		    AVOID_EXECUTE = true;
		    IGNORE_NEXT_WORD = false; //acknowledge next word.
		    //replace carry over with (True) or (False) depending on previous_execute
		    if (PREVIOUS_EXECUTE == 0){
		        (*it)->set_word(new Arg("(True)") );
		    } else if (PREVIOUS_EXECUTE == 1) {
		        (*it)->set_word(new Arg("(False)") );
		    } else if (PREVIOUS_EXECUTE == -1) {
		        (*it)->set_word(new Arg("(Invalid)") );
		    }
		}
		
		if (AVOID_EXECUTE == false){
		    //std::cout << "executing '" << (*it)->get_word() << "'" << endl;
		    PREVIOUS_EXECUTE = dynamic_cast<Command*>(*it)->execute(); //returns 0 if succesful, 1 if failed, -1 if invalid.
	    	    if (PREVIOUS_EXECUTE == -1){
		        std::cout << "Invalid Command: \"" << (*it)->get_word() << "\"" <<  std::endl;
		    	PREVIOUS_EXECUTE = 1; //treat invalid commands as failures
		    }

		} else {
		    if (IGNORE_NEXT_WORD == true){
		    	if ( (it+1) != word_list.end()){
		        	it++; //skip this word and move to the next
		    	}
		    	IGNORE_NEXT_WORD = false;
		    }
		    AVOID_EXECUTE = false;
		}
		//std::cout << "PREVIOUS_EXECUTE: " << PREVIOUS_EXECUTE << std::endl;
	    }
	    for (auto strit = strats.begin(); strit != strats.end(); strit++){(*strit)->set_status(false);} //set all status of strats to false since we are done with the line
	    //cout << "This sequence returns: " << PREVIOUS_EXECUTE << endl;
	    return PREVIOUS_EXECUTE;
        };

};

#endif
