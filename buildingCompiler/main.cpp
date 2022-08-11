#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>


// The Lexer returns tokens [0-255] if it is an unknown character,
// or it is these negative numbers
enum Token {
    tok_eof = -1,
    
    // commands
    tok_def = -2,
    tok_extern = -3,
    
    // primary
    tok_identifier = -4,
    tok_number = -5,
};

static std::string IdentifierStr;
static double NumVal;

// return next token from standard input
static int gettok() {
    static int LastChar = ' ';
    
    // skip white spaces
    while (isspace(LastChar))
        LastChar = getchar();
    
    if (isalpha(LastChar)) {
        IdentifierStr += LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr +=LastChar;
        
        if (IdentifierStr == "def")
            return tok_def;
        if (IdentifierStr == "extern")
            return tok_extern;
        return tok_identifier;
    }
   
    // this can't handle 123.323.123 etc
    // TODO handle multiple .
    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr+=LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');
        
        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }
        
    if (LastChar == '#') {
        // comment until end of line
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
        
        if (LastChar != EOF)
            return gettok();
    }
    
    if (LastChar == EOF)
        return tok_eof;
    
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}
    
    
    
    
    
