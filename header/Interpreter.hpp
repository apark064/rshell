#include <iostream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>

class Interpreter {
    private:
        std::vector<Word*> word_list;
    public:
        Interpreter() {};
        Interpreter(std::vector<Word*> word_list) {this->word_list = word_list;};
        void read_line(){
            std::string s;
            std::getline(std::cin, s);
            boost::char_separator<char> delim("||&&;# ", "", boost::keep_empty_tokens); //needs multiple delimiter instead of one?
            boost::tokenizer<boost::char_separator<char>> token(s,delim); //
            // std::vector<std::string> without_semicolon;  //tried to separate into each delimiter on its own? but how to get rid of comments

            for(auto it = tok.begin(); it != tok.end(); ++it){   
            std::cout << *it << std::endl;
            // without_semicolon.push_back(*it);   
            }

            // for(int i = 0; i < without_semicolon.size(); ++i){
            //     std::cout << without_semicolon.at(i) << std::endl;
            // }
        };

};
