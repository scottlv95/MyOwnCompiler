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

namespace {
// base class
class ExprAST {
    public : virtual ~ExprAST() {}
};
   
// for numbers "1.0"
class NumberExprAST : public ExprAST {
    double Val;

public:
NumberExprAST(double Val) : Val(Val) {}
};

// for referencing a variable
class VariableExprAST : public ExprAST {
    std:: string Name;

public:
    VariableExprAST(const std:: string &Name) : Name(Name) {};
};

// for binary operator
class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;
    
public :
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS): Op(op), LHS(std::move(LHS)),RHS(std:: move(RHS)) {}
};

// for function calls
class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee,
                std:: vector<std::unique_ptr<ExprAST>> Args):
    Callee(Callee), Args(std::move(Args)) {}
};

// this represents "prototype" of a function

class PrototypeAST {
    std::string Name;
    std::vector<std::string> Args;
public:
    PrototypeAST(const std:: string &name, std:: vector<std::string> Args): Name(name), Args(std::move(Args)) {}
    const std:: string &getName() const {return Name; }
};

// function definition FUTURE TODO: can add type field
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body): Proto(std::move(Proto)),Body(std::move(Body)) {}
};
}





    
    
