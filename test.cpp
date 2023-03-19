#include "Lexer.h"
#include "Grammar.h"

int main()
{

    std::string strTest = "The quick brown fox jumps over the lazy dog";
    Lexer* lex = new Lexer();

    lex->setLine(strTest);

    std::cout << lex->getToken() << "\n";
    std::cout << lex->readToken() << "\n";
    std::cout << lex->readToken() << "\n";
    std::cout << lex->readToken() << "\n";
    std::cout << lex->readToken() << "\n";

    return 0;
}