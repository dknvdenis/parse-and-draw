#pragma once

#include <string>

enum class TokenType_t
{
    unknown,
    whitespace,
    number,
    op, // operator
    leftParen,
    rightParen,
    func, // function
    var // variable
};

struct Token
{
    explicit Token()
        : type(TokenType_t::unknown), posInLine(0)
    {}

    explicit Token(const std::string &tokenName, TokenType_t type_, int posInLine_)
        : name(tokenName), type(type_), posInLine(posInLine_)
    {}

    Token(const std::string &tokenName, TokenType_t type_)
        : Token(tokenName, type_, 0)
    {}

    std::string name;
    TokenType_t type;
    int posInLine;
};
