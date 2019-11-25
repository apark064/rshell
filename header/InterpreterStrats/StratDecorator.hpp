#ifndef __STRATDECORATOR_HPP__
#define __STRATDECORATOR_HPP__

#include "../Interpreter.hpp"
#include "InterpreterStrat.hpp"

#include <string>
#include <vector>

class StratDecorator : public InterpreterStrat{
        private:
		InterpreterStrat* Base;
                bool status;
                std::string character;
		vector<int> calls;
        public:
                virtual vector<int>  interpret(std::string token){return Base->interpret(token);}
                virtual vector<int> execute (){return Base->execute();}//this is overwritten in subclasses
                virtual bool get_status() {return Base->get_status();}
		virtual std::string get_character() {return Base->get_character();}
};

#endif
