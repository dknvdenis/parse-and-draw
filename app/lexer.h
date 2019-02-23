#pragma once

#include <string>
#include <unordered_set>
#include "iLexer.h"

class Lexer : public ILexer
{
public:
    explicit Lexer(const std::string &input, char decimalPoint);

public:
    Token nextToken() override;
    bool isDone() const override;
    void reset() override;

private:
    std::string _input;
    int _pos;
    char _decimalPoint;

    static std::unordered_set<char> _ops;
    static std::unordered_set<std::string> _funcs;

private:
    bool isSplitSymbol(char s) const;
    Token createToken(const std::string &name, int pos) const;

    bool isNumber(const std::string &str) const;
    bool isOperator(char s) const;
    bool isLeftParen(char s) const;
    bool isRightParen(char s) const;
    bool isFunction(const std::string &str) const;
    bool isVariable(const std::string &str) const;
};
