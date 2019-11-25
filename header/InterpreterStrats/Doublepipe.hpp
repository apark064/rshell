#ifndef __DOUBLEPIPE_HPP__
#define __DOUBLEPIPE_HPP__


#include "../Interpreter.hpp"
#include "InterpreterStrat.hpp"

#include <string>
#include <vector>
#include <iostream>

class Doublepipe : public StratDecorator{

        private:
		InterpreterStrat* Base;
                bool status;
                std::string character;
        public:
                Doublepipe(InterpreterStrat* Base){this->Base = Base;}
                virtual std::vector<int> interpret(std::string token){return this->Base->interpret(token);}
                virtual std::vector<int> execute(){
		    std::vector<int> calls;
		    calls.push_back(4); //enable tokenReset
		    this->set_status(false);
		    return calls;
		}
                virtual bool get_status() {return Base->get_status();}
                virtual bool set_status(bool value) {Base->set_status(value);}
		virtual std::string get_character() {return Base->get_character();}

};

#endif

