#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <fstream>

namespace special
{
    const char DELIMITER = ' ';
    const char EOL       = '@';
    const char EPSILON   = '#';
}

class Lexer     // Lexical analyzer
{
    
    public:
    
    Lexer();
    
    std::string peekToken();
    std::string readToken();
    
    std::string getToken();
    std::string getLine();
    
    int getTokenStart();
    int getTokenEnd();
    
    void setLine(std::string ln);
    void resetIndices();
    
    bool hasTokens();
    
    protected:
    
    bool checkEOL();
    bool checkDLM();
    void initToken();    
    
    private:
    
    bool lineHasTokens;
    int tkStart;
    int tkEnd;
    std::string line;
    
};

class Flexer : public Lexer    // File lexical analyzer
{
    
    public:
    
    Flexer();
    Flexer(std::string fl);
    
    void fNextLine();
    
    void fOpen(std::string fl);
    bool fValid();
    
    protected:
    
    bool checkEOF();
    void fInitLine();  
    
    private:
    
    std::ifstream file;
    
};

class Plexer : public Flexer    // Program lexical analyzer
{
    
    public:

    protected:
    
    private:
    
};

#endif