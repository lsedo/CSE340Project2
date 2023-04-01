#include "Grammar.h"
#include "Lexer.h"

std::string Grammar::getNonterminals()
{
    std::set<std::string>::iterator itr;
    std::string str = "";

    std::cout << nonterminals.size() << "\n";

    for(itr = nonterminals.begin(); itr != nonterminals.end(); itr++)
    {
        str += *itr + "\n";
    }

    return str;
}

std::string Grammar::getTerminals()
{
    std::set<std::string>::iterator itr;
    std::string str = "";

    std::cout << terminals.size() << "\n";

    for(itr = terminals.begin(); itr != terminals.end(); itr++)
    {
        str += *itr + "\n";
    }

    return str;
}

std::string Grammar::getRules()
{
    std::string str = "";

    for(int i = 0; i < rules.size(); i++)
    {
        Rule rl = rules.at(i);

        str += getNonterminal(rl.pPair.first) + " " + special::PRODUCTION;

        for(int j = 0; j < rl.pSize; j++)
        {
            int dist = rl.pPair.second[j];
            
            if(dist < 0)
            {
                dist = -1 + (-1 * dist);
                str += " " + getNonterminal(dist);
            }
            else
            {
                str += " " + getTerminal(dist);
            }
        }

        str += "\n";
    }

    return str;
}

std::string Grammar::getFirstSets()
{
    std::string str = "";

    for(int i = 0; i < nonterminals.size(); i++)
    {    
        auto ntItr = nonterminals.begin(); 
        std::advance(ntItr, i);

        str += "FIRST(" + *(ntItr) + ") =";

        for(auto itr = firstSets[i].begin(); itr != firstSets[i].end(); itr++)
        {
            int dist = *itr;
            auto tItr = terminals.begin();

            std::advance(tItr, dist);

            str += " " + *(tItr);
        }

        str += "\n";
    }

    return str;
}

std::string Grammar::getFollowSets()
{
    std::string str = "";

    for(int i = 0; i < nonterminals.size(); i++)
    {    
        auto ntItr = nonterminals.begin(); 
        std::advance(ntItr, i);

        str += "FOLLOW(" + *(ntItr) + ") =";

        for(auto itr = followSets[i].begin(); itr != followSets[i].end(); itr++)
        {
            int dist = *itr;
            auto tItr = terminals.begin();
            
            if(dist == -1)
            {
                str += " ";
                str += special::EI;
            }
            else
            {
                std::advance(tItr, dist);
                str += " " + *(tItr);
            }
        }

        str += "\n";
    }

    return str;
}

int Grammar::getNonterminal(std::string tk)
{
    int setWidth = std::distance(nonterminals.begin(), nonterminals.end());
    int dist = std::distance(nonterminals.begin(), nonterminals.find(tk));

    if(dist == setWidth)
    {
        return -1;
    }
    else
    {
        return dist;
    }
}

int Grammar::getTerminal(std::string tk)
{
    int setWidth = std::distance(terminals.begin(), terminals.end());
    int dist = std::distance(terminals.begin(), terminals.find(tk));

    if(dist == setWidth)
    {
        return -1;
    }
    else
    {
        return dist;
    }
}

std::string Grammar::getNonterminal(int dist)
{
    auto setFirst = nonterminals.begin();

    return *(std::next(setFirst, dist));
}

std::string Grammar::getTerminal(int dist)
{
    auto setFirst = terminals.begin();

    return *(std::next(setFirst, dist));
}

void Grammar::initFromFile(std::string fl)
{
    initSymbols(fl);
    initRules(fl);
    initFirstSets();
    initFollowSets();
}

void Grammar::initSymbols(std::string fl)
{
    Flexer* flex = new Flexer(fl);

    initNonTerminals(flex);
    initTerminals(flex);
}

void Grammar::initNonTerminals(Flexer* flex)
{
    std::string ntToken = flex->getToken();

    while(ntToken.front() != special::EL)    // reads tokens into nonterminals set until EOL
    {
        nonterminals.insert(ntToken);
        ntToken = flex->readToken();
    }

    flex->fNextLine();
}

void Grammar::initTerminals(Flexer* flex)
{
    std::string ntToken = flex->getToken();
    bool tRead = false;

    while(!flex->checkEOF())
    {
        while(ntToken.front() != special::EL)    // reads tokens into nonterminals set until EOL
        {
            if(tRead)
            {
                if(getNonterminal(ntToken) == -1 && ntToken.at(0) != special::OR)
                {
                   terminals.insert(ntToken); 
                }   
            }

            if(ntToken.compare(special::PRODUCTION) == 0)
            {
                tRead = true;
            }

            ntToken = flex->readToken();
        }

        flex->fNextLine();
        ntToken = flex->getToken();
        tRead = false;
    }

    ntToken = "";
    ntToken += special::EPSILON;
    terminals.insert(ntToken);
}

std::set<int>* Grammar::getFirstSet(int index)
{
    return NULL;
}

std::set<int>* Grammar::getFollowSet(int index)
{
    return NULL;   
}

std::set<int>* Grammar::getFirstSet(std::string ntToken)
{
    return NULL;    
}

std::set<int>* Grammar::getFollowSet(std::string ntToken)
{
    return NULL;    
}

void Grammar::initRules(std::string fl)
{
    Flexer* flex = new Flexer(fl);
    Lexer* miniLex = new Lexer();

    flex->fNextLine();


    while(!flex->checkEOF())
    {
        std::string rToken = flex->getToken();
        Rule rl;
        std::pair<int,int*> rPair;

        bool pRead = false;
        int pIndex = 0;

        while(!flex->checkEOL())
        {
            if(rToken.compare(special::PRODUCTION) == 0 || rToken.at(0) == special::OR)
            {
                if(rToken.at(0) == special::OR)
                {
                    rl.pPair = rPair;
                    rules.push_back(rl);
                    pIndex = 0;
                }

                std::string pToken = flex->getLine().substr(flex->getTokenEnd() + 1);
                miniLex->setLine(pToken);

                int pSize = getProductionSize(miniLex);

                if(pSize == 0)
                {
                    rl.pSize = 1;
                    rPair.second = new int(rl.pSize);
                    rPair.second[0] = 0;

                }
                else
                {
                    rl.pSize = pSize;
                    rPair.second = new int(rl.pSize);
                }

                pRead = true;
            }
            else
            {
                if(pRead)
                {
                    int dist;

                    if(getNonterminal(rToken) != -1)
                    {
                        dist = -1 + (getNonterminal(rToken) * -1);
                    }
                    else if(getTerminal(rToken) != -1)
                    {
                        dist = getTerminal(rToken);
                    }

                    rPair.second[pIndex] = dist;
                    pIndex++;
                }
                else
                {
                    rPair.first = getNonterminal(rToken);
                }
            }

            rToken = flex->readToken();
        }

        rl.pPair = rPair;
        rules.push_back(rl);

        flex->fNextLine();
    }
}

void Grammar::initFirstSets()
{
    firstSets = new std::set<int>[nonterminals.size()];

    for(int i = 0; i < nonterminals.size(); i++)
    {
        if(hasEpsilonRule(i))
        {
            firstSets[i].insert(0);
        }   
    }

    for(int x = 0; x < nonterminals.size(); x++)
    {
        for(int i = 0; i < rules.size(); i++)
        {
            Rule rl = rules.at(i);
            int nt = rl.pPair.first;
            
            for(int j = 0; j < rl.pSize; j++)
            {
                int p = rl.pPair.second[j];

                if(p > 0)
                {
                    firstSets[rl.pPair.first].insert(p);
                }
                else if(p < 0)
                {
                    mergeFirstSets(nt, -1 + (-1 * p), false);
                }

                if(hasEpsilonInFirst(j) && j == rl.pSize - 1)
                {
                    firstSets[nt].insert(0);
                }
                else if(!hasEpsilonInFirst(j))
                {
                    j = rl.pSize;
                }
            }
        }
    }
}

void Grammar::initFollowSets()
{
    followSets = new std::set<int>[nonterminals.size()];
    followSets[0].insert(-1);

    for(int i = 0; i < nonterminals.size(); i++)
    {
        for(int j = 0; j < rules.size(); j++)
        {
            Rule rl = rules.at(j);
            int nt = rl.pPair.first;
            
            if(rl.pSize > 1)
            {
                for(int k = 0; k < rl.pSize; k++)
                {
                    int p = rl.pPair.second[k];

                    if(p < 0)
                    {
                        p = -1 + (p * -1);

                        if(k == rl.pSize - 1)   // A -> aB
                        {
                            mergeFollowSets(p, nt, false);
                        }
                        else                    // A -> aBb
                        {
                            int b = rl.pPair.second[k + 1];

                            if(b > 0)
                            {
                                followSets[p].insert(b);
                            }
                            else
                            {
                                b = -1 + (b * -1);

                                mergeFirstFollow(p, b, false);

                                if(hasEpsilonInFirst(b))
                                {
                                    mergeFollowSets(p, nt, false);
                                }
                            }
                        }
                    }
                }
                
            }
        }
    }
}

void Grammar::mergeFirstSets(int set1, int set2, bool includeEpsilon)
{
    auto itr2Begin = firstSets[set2].begin();
    auto itr2End = firstSets[set2].end();

    if(!includeEpsilon)
    {
        if(firstSets[set2].find(0) != firstSets[set2].end())
        {
            itr2Begin++;
        }
    }

    firstSets[set1].insert(itr2Begin, itr2End);
}

void Grammar::mergeFollowSets(int set1, int set2, bool includeEpsilon)
{
    auto itr2Begin = followSets[set2].begin();
    auto itr2End = followSets[set2].end();

    if(!includeEpsilon)
    {
        if(followSets[set2].find(0) != followSets[set2].end())
        {
            itr2Begin++;
        }
    }

    followSets[set1].insert(itr2Begin, itr2End);
}

void Grammar::mergeFirstFollow(int set1, int set2, bool includeEpsilon)
{
    auto itr2Begin = firstSets[set2].begin();
    auto itr2End = firstSets[set2].end();

    if(!includeEpsilon)
    {
        if(firstSets[set2].find(0) != firstSets[set2].end())
        {
            itr2Begin++;
        }
    }

    followSets[set1].insert(itr2Begin, itr2End);
}

bool Grammar::hasEpsilonInFirst(int dist)
{
    return (firstSets[dist].find(0) != firstSets[dist].end());
}

bool Grammar::hasEpsilonInFollow(int dist)
{
    return (firstSets[dist].find(0) != followSets[dist].end());
}

bool Grammar::hasEpsilonRule(int dist)
{
    for(int i = 0; i < rules.size(); i++)
    {
        if(rules.at(i).pPair.first == dist)
        {
            if(rules.at(i).pSize == 1 && rules.at(i).pPair.second[0] == 0)
            {
                return true;
            }
        }
    }

    return false;
}

int Grammar::getProductionSize(Lexer* miniLex)
{
    int pSize = 0;

    while(!miniLex->checkEOL() && miniLex->getToken().at(0) != special::OR)
    {
        pSize++;

        miniLex->readToken();
    }

   return pSize;
}

void Grammar::debug()
{

}
