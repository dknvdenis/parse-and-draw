#pragma once

#include <memory>
#include <unordered_map>
#include <stack>
#include "iLexer.h"
#include "rpn.h"

class InfixToRPN
{
public:
    explicit InfixToRPN(std::shared_ptr<ILexer> lexer);

public:
    RPN_t convert();

private:
    void parse(const Token &token, RPN_t &rpn, std::stack<Token> &stack);

private:
    std::shared_ptr<ILexer> _lexer;
    static std::unordered_map<std::string, int> _priority;
};
