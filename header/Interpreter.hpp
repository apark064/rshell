/* Note: This code could definitely be improved. There are a lot of "hard-coded" cases in here that could be replaced with more re-usable code.
 * 	 Also, theres some conditional statements (mainly in the double-character cases) that could be less redundant.
 * */

#include <iostream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>

#include "Word.hpp"
#include "Command.hpp"
#include "Exec.hpp"
#include "Arg.hpp"
#include "Connector.hpp"

class Interpreter {
    private:
        std::vector<Word*> word_list; //will only contain Command Objects and Connector Objects
    public:
        Interpreter() {};
        Interpreter(std::vector<Word*> word_list) {this->word_list = word_list;};
        int read_line(){ //returns 0 if not exiting. returns 1 if exiting
            std::string line;
	    std::cout << "$ ";
            std::getline(std::cin, line);
            boost::char_separator<char> delim("", "|&;#\" "); //if the interpreter encounters any of the characters in the 2nd arg, it tokenizes the character individually
            boost::tokenizer<boost::char_separator<char>> tokens(line,delim); //TOKENINZE USER INPUT
		
	    //prepatory vars
	    std::string prev_token;
	    bool tokenReset = false; //set this to true to reset prev_token at the end of the iteration
	    bool ignore = false; //if this is set to true, the interpreter ignores the token
	    bool string = false; //if this is set to true, the interpreted word is considered part of a string"
	    unsigned wordCount = 0; //the number of words interpreted. resets when token is resetted
            Command* command = new Command();

	    //INTERPRET TOKENS
	    for(auto it = tokens.begin(); it != tokens.end(); it++){   
		std::string token = *it;
		//HANDLING FOR COMMENTS
		if (token == "#"){
		    ignore = true;
		}
		
		//IF NOT IGNORING TOKENS
		if (ignore == false){
		    //HANDLING FOR DOUBLE-CHARACTER CASES
		    if ( (prev_token == "|") && (string == false) ){
		        if (token == "|"){ //THE CURRENT TOKEN CREATES A ||
		    	    wordCount--; //subtract the count we performed for the previous char;
			    tokenReset = true;
		        } else { //if the user only used one |
			    //std::cout << "INVALID CONNECTOR" << std::endl;
		        }
		    } else if ( (prev_token == "&") && (string == false) ){ 
		        if (token == "&"){ //THE CURRENT TOKEN CREATES A &&
                            wordCount--; //subtract the count we performed for the previous char
			    tokenReset = true;
		        } else { //if the user only used one &
			   //std::cout << "INVALID CONNECTOR" << std::endl;
		        }
		    }
		    //HANDLING FOR SINGLE-CHARACTER CASES
		    if ( (token == ";") && (string == false ) ){ //SEMICOLON
		        tokenReset = true;
		    } else if (token == "\""){ //DOUBLEQUOTE
		        if (string == false){ //enable string
			    string = true;
			    //set up string
			    token = "";
			    prev_token = "";
			} else { //disable string
			    string = false;
                            token = prev_token;       
			}
		    }
		    
		    if (tokenReset == false){
		    	if (string == false){
			    if (token != " "){ //ignore space tokens
			        //HANDLING FOR POTENTIAL DOUBLE-CHAR CONNECTORS  
			        if (token == "|"){
			        } else if (token == "&"){
			        }
			        //HANDLING FOR NORMAL WORDS
			        else {
		    	            if (wordCount == 0){ //APPENED A EXEC
					if (token == "exit"){ //EXIT CONDITION. EXIT RSHELL
					    return 1;
					} else {
					    command->add_word(new Exec(token));
					}
				    	//std::cout << "\"" << token << "\" was appended as an EXEC" << std::endl;
				    } else { //APPEND AN ARG
					command->add_word(new Arg(token));
				    	//std::cout << "\"" << token << "\" was appended as an ARG" << std::endl;
				    }
			        }
			        wordCount++;    
			        prev_token = token;
			    }	
                        }
			else {
			    prev_token.append(token); //just append to prev_token
			    //wordCount isn't updated to signify that we're still on the same word
		    	}
		    } 
		    //RESET TOKENS IF NECESSARY
		    else {
			//HANDLING FOR DOUBLE CHAR CONNECTORS
			if (token != ";"){ //if our token is a double char connector
			    token.append(token); //double the char
			}
			//append the built command we have made and instantiate a new command object to append to
			word_list.push_back(command);
			command = new Command();
			
			//append our connector
			word_list.push_back(new Connector(token));
			//std::cout << "\"" << token << "\" was appended as a CONNECTOR" << std::endl;
			
			//reset
		        prev_token = "";
		        tokenReset = false;
		        wordCount = 0;
		    }
		}
            }
	    word_list.push_back(command); //push the last command in
	    ignore = false;
	    //EXECUTE COMMANDS
	    for (auto it = word_list.begin(); it != word_list.end(); it++){
		//std::cout << (*it)->get_word() << std::endl;
		dynamic_cast<Command*>(*it)->execute();		
	    }
	    word_list.clear();
	    return 0;
        };

};
