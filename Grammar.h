#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <utility>
#include <vector>
#include <string>
#include <set>
#include "Lexer.h"

struct Rule
{
    std::pair<int, int*> pPair;
    int pSize;
};

class Grammar
{
    public:

    void initFromFile(std::string fl);

    std::string getNonterminals();     // debug functions
    std::string getTerminals();
    std::string getRules();
    std::string getFirstSets();
    std::string getFollowSets();

    std::set<int>* getFirstSet(int index);
    std::set<int>* getFollowSet(int index);
    std::set<int>* getFirstSet(std::string ntToken);
    std::set<int>* getFollowSet(std::string ntToken);
    
    int getNonterminal(std::string tk);
    int getTerminal(std::string tk);
    std::string getNonterminal(int dist);
    std::string getTerminal(int dist);
    
    void debug();

    protected:

    void initSymbols(std::string fl);
    void initRules(std::string fl);
    void initFirstSets();
    void initFollowSets();

    void mergeFirstSets(int set1, int set2, bool includeEpsilon);
    void mergeFollowSets(int set1, int set2, bool includeEpsilon);
    void mergeFirstFollow(int set1, int set2, bool includeEpsilon);

    bool hasEpsilonInFirst(int dist);
    bool hasEpsilonInFollow(int dist);
    bool hasEpsilonRule(int dist);

    private:

    void initNonTerminals(Flexer* flex);
    void initTerminals(Flexer* flex);

    int getProductionSize(Lexer* miniLex);

    std::vector<Rule> rules;
    std::set<std::string> nonterminals;
    std::set<std::string> terminals;
    std::set<int>* firstSets;
    std::set<int>* followSets;

};

#endif