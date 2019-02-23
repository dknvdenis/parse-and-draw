#include "lexer.h"
#include <algorithm>
#include <cctype>

std::unordered_set<char> Lexer::_ops = {
    '+',
    '-',
    '*',
    '/',
    '^'
};

std::unordered_set<std::string> Lexer::_funcs = {
    "sin",
    "cos",
    "tan",
    "cot",
    "minus",
    "abs"
};

Lexer::Lexer(const std::string &input, char decimalPoint)
    : ILexer(),
      _input(input),
      _pos(0),
      _decimalPoint(decimalPoint)
{

}

Token Lexer::nextToken()
{
    if (isDone())
        return Token();

    if (isSplitSymbol(_input[_pos]))
    {
        int pos = _pos;
        return createToken(std::string(&_input[_pos++], 1), pos);
    }

    int begin = _pos;
    for (; _pos <= _input.size(); _pos++)
    {
        if (_pos == _input.size() || isSplitSymbol(_input[_pos]))
            return createToken(std::string(&_input[begin], _pos - begin), begin);
    }

    return Token();
}

bool Lexer::isDone() const
{
    return (_pos >= _input.size());
}

void Lexer::reset()
{
    _pos = 0;
}

bool Lexer::isSplitSymbol(char s) const
{
    return (s == ' ' || s == '\n'
            || isOperator(s) || isLeftParen(s)|| isRightParen(s));
}

Token Lexer::createToken(const std::string &name, int pos) const
{
    Token token;
    token.name = name;
    token.posInLine = pos;

    if (name.size() == 1 && name[0] == ' ')
        token.type = TokenType_t::whitespace;
    else if (isNumber(name))
        token.type = TokenType_t::number;
    else if (name.size() == 1 && isOperator(name.front()))
        token.type = TokenType_t::op;
    else if (name.size() == 1 && isLeftParen(name.front()))
        token.type = TokenType_t::leftParen;
    else if (name.size() == 1 && isRightParen(name.front()))
        token.type = TokenType_t::rightParen;
    else if (isFunction(name))
        token.type = TokenType_t::func;
    else if (isVariable(name))
        token.type = TokenType_t::var;
    else
        token.type = TokenType_t::unknown;

    return token;
}

bool Lexer::isNumber(const std::string &str) const
{
    auto iter = std::find_if_not(str.cbegin(), str.cend(),
                                 [=] (const char c) {
        return (std::isdigit(c) || c == _decimalPoint);
    });

    return (!str.empty() && iter == str.cend());
}

bool Lexer::isOperator(char s) const
{
    return (_ops.find(s) != _ops.cend());
}

bool Lexer::isLeftParen(char s) const
{
    return (s == '(');
}

bool Lexer::isRightParen(char s) const
{
    return (s == ')');
}

bool Lexer::isFunction(const std::string &str) const
{
    return (_funcs.find(str) != _funcs.cend());
}

bool Lexer::isVariable(const std::string &str) const
{
    if (isFunction(str))
        return false;

    auto iter = std::find_if_not(str.cbegin(), str.cend(), [] (const char c) {
        return std::isalpha(c);
    });

    return (!str.empty() && iter == str.cend());
}
