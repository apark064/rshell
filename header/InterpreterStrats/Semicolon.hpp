#ifndef __SEMICOLON_HPP__
#define __SEMICOLON_HPP__

#include "StratDecorator.hpp"
#include "../Interpreter.hpp"

#include <string>
#include <iostream>
#include <vector>

class Semicolon : public StratDecorator{
        private:
                InterpreterStrat* Base;
        public:	
		// NEW / OVERWRITTEN
		Semicolon(InterpreterStrat* Base){this->Base = Base;}
                virtual std::vector<int> execute(){
			this->set_status(false); //reset status
			return {4};// set tokenReset to true
		};
		
		//NOT OVERWRITTEN
		virtual std::vector<int> interpret(std::string token){return Base->interpret(token);}
                virtual bool get_status() {return Base->get_status();}
		virtual bool set_status(bool value) {Base->set_status(value);}
		virtual std::string get_character() {return Base->get_character();}
};

#endif

