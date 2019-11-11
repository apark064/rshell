#include <iostream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>

class Interpreter {
    private:
        vector<Word*> word_list;
    public:
        Interpreter() {};
        void read_line(){
            std::string s;
            boost::tokenizer<> token(s);
        };

};
