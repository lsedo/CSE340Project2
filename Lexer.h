#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <fstream>

namespace special
{
    const char          DELIMITER     = ' ';
    const std::string   EF            = "@@";
    const char          EL            = '@'; 
    const char          EI            = '$';
    const char          EPSILON       = '#';
    const char          OR            = '|';
    const std::string   PRODUCTION    = "->";   
};

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
    bool checkEOL();
    bool checkDLM();
    
    protected:
      
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
    bool checkEOF();
    
    protected:
    

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