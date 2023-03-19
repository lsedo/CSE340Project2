#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <utility>
#include <vector>
#include <string>
#include "Lexer.h"

struct
{
    std::pair<int, int*> production;
    int size;
} Rule;

class Grammar
{

    public:

    bool initFromFile(std::string fl);

    void getNonTerminals();
    void getTerminals();

    protected:

    bool initSymbols(std::string fl);

    bool initRules(std::string fl);

    private:

    bool initNonTerminals(Flexer* flex);
    bool initTerminals(Flexer* flex);

    std::vector<struct Rule> rules;
    std::set<std::string> nonterminals;
    std::set<std::string> terminals;

};

#endif