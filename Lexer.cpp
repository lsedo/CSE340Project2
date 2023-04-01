#include <iostream>
#include <string>
#include <fstream>
#include "Lexer.h"

                                        // Lexical Analyzer methods

Lexer::Lexer()
{
    resetIndices();
}

std::string Lexer::peekToken()
{
    bool tkFound = false;
    bool nextStartFound = false;
    int originStart = tkStart;
    int originEnd= tkEnd;
    
    while(!tkFound)
    {
        if(checkEOL())
        {
            tkFound = true;
        }
        else
        {
            tkEnd++;
            
            if(checkDLM())
            {
                if(nextStartFound)
                {
                    tkFound = true;
                }
                
                if(checkEOL())
                {
                    tkFound = true;
                    tkEnd = originEnd; // case where last token is followed by several delimiters
                }
            }
            else
            {
                if(!nextStartFound)
                {
                    nextStartFound = true;
                    tkStart = tkEnd;
                }
            }
        }
    }

    if(checkEOL())
    {
        std::string tkEOL(1, special::EL);
        return tkEOL;
    }
    else
    {  
        std::string token = getToken();
        tkStart = originStart;
        tkEnd = originEnd;
    
        return token;
    }
    
  
}

std::string Lexer::readToken()
{
    bool tkFound = false;
    bool nextStartFound = false;
    int iterations = 0;
    int originStart = tkStart;
    int originEnd= tkEnd;
    
    while(!tkFound)
    {
        if(checkEOL())
        {
            tkFound = true;
            nextStartFound = true;
        }
        else
        {
            iterations++;
            tkEnd++;
            
            if(checkDLM())
            {
                if(nextStartFound)
                {
                    tkFound = true;
                }
                
                if(checkEOL())
                {
                    tkFound = true;
                    tkEnd = tkEnd - iterations; // case where last token is followed by several delimiters
                }
            }
            else
            {
                if(!nextStartFound)
                {
                    nextStartFound = true;
                    tkStart = tkEnd;
                }
            }
        }
    }
    
    if(checkEOL())
    {
        std::string tkEOL(1, special::EL);
        return tkEOL;
    }
    else
    {
        return getToken();
    }
    
}

std::string Lexer::getToken()
{
    return line.substr(tkStart, tkEnd - tkStart);
}

std::string Lexer::getLine()
{
    return line;
}

int Lexer::getTokenStart()
{
    return tkStart;
}

int Lexer::getTokenEnd()
{
    return tkEnd;
}

void Lexer::setLine(std::string ln)
{
    line = ln;
    resetIndices();
    initToken();
}

void Lexer::resetIndices()
{
    tkStart = 0;
    tkEnd = 0;
}

bool Lexer::hasTokens()
{
    return lineHasTokens;
}

bool Lexer::checkEOL()
{
    return (tkEnd >= line.length() - 1) || (line.at(tkEnd) == special::EL);
}

bool Lexer::checkDLM()
{
    if(checkEOL())
    {
        return false;
    }
    else
    {
        return (line.at(tkEnd) == special::DELIMITER); 
    }
}

void Lexer::initToken()
{
    bool dlmFound = false;
    bool dlmFirstChar = false;
    
    while(!dlmFound)
    {
        if(checkEOL())
        {
            dlmFound = true;
        }
        else
        {
            if(checkDLM())
            {
                if(tkEnd == 0)
                {
                    dlmFirstChar = true;
                }
                
                if(!dlmFirstChar)
                {
                    dlmFound = true;
                }
                else if(dlmFirstChar)
                {
                    tkEnd++;
                }
            }
            else
            {
                lineHasTokens = true;
                
                if(dlmFirstChar)
                {
                    tkStart = tkEnd;
                    dlmFirstChar = false;
                }
                
                tkEnd++;
            }
        }
    }
}

                                        // File Lexical Analyzer methods

Flexer::Flexer()
{
    resetIndices();
}

Flexer::Flexer(std::string fl)
{
    fOpen(fl);
    fInitLine();
}

void Flexer::fNextLine()
{
    if(!checkEOF())
    {
        fInitLine();
    }
}

void Flexer::fOpen(std::string fl)
{
    file.open(fl);
}

bool Flexer::fValid()
{
    return file.is_open();
}

bool Flexer::checkEOF()
{
    if(getLine().compare(special::EF) == 0)
    {
        return true;
    }

    return !file.good();
}

void Flexer::fInitLine()
{
    std::string ln;
    std::getline(file, ln);
    
    setLine(ln);
}