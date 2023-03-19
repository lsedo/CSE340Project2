#include "Grammar.h"
#include "Lexer.h"

bool Grammar::initFromFile(std::string fl)
{
    initSymbols(fl);
    initRules(fl);

    return false;
}

bool Grammar::initSymbols(std::string fl)
{
    Flexer* flex = new Flexer(fl);

    initNonTerminals(flex);
    initTerminals(flex);

    return false;
}

bool Grammar::initNonTerminals(Flexer* flex)
{
    return false;
}

bool Grammar::initTerminals(Flexer* flex)
{
    return false;
}

bool Grammar::initRules(std::string fl)
{
    return false;
}

