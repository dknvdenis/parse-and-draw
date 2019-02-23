#include "infixToRPN.h"
#include "appExceptions.h"

std::unordered_map<std::string, int> InfixToRPN::_priority = {
    {"+", 1},
    {"-", 1},
    {"*", 2},
    {"/", 2},
    {"^", 3}
};

InfixToRPN::InfixToRPN(std::shared_ptr<ILexer> lexer)
    : _lexer(lexer)
{

}

RPN_t InfixToRPN::convert()
{
    _lexer->reset();

    RPN_t result;
    std::stack<Token> stack;

    Token prevToken;
    while (!_lexer->isDone())
    {
        Token token = _lexer->nextToken();
        if (token.type == TokenType_t::unknown)
            throw TokenException(token, "Неизвестный токен");

        if (token.type == TokenType_t::whitespace)
            continue;

        if ((prevToken.type == TokenType_t::unknown || prevToken.type == TokenType_t::leftParen)
                && token.name == "-")
        {
            token = Token("minus", TokenType_t::func, token.posInLine);
        }

        parse(token, result, stack);
        prevToken = token;
    }

    while (!stack.empty())
    {
        Token &t = stack.top();
        if (t.type != TokenType_t::op && t.type != TokenType_t::func)
            throw TokenException(t, "В выражении есть непарные скобки");

        result.push(t);
        stack.pop();
    }

    return result;
}

void InfixToRPN::parse(const Token &token, RPN_t &rpn, std::stack<Token> &stack)
{
    switch (token.type)
    {
    case TokenType_t::number:
    case TokenType_t::var:
        rpn.push(token);
        break;

    case TokenType_t::func:
    case TokenType_t::leftParen:
        stack.push(token);
        break;

    case TokenType_t::op:
        while (!stack.empty()
               && (_priority[stack.top().name] >= _priority[token.name]
                   || stack.top().type == TokenType_t::func))
        {
            rpn.push(stack.top());
            stack.pop();
        }

        stack.push(token);
        break;

    case TokenType_t::rightParen: {
        bool e = true;
        while (!stack.empty())
        {
            Token t = stack.top();
            stack.pop();

            if (t.type == TokenType_t::leftParen)
            {
                e = false;
                break;
            }

            rpn.push(t);
        }

        if (e) throw TokenException(token, "В выражении есть непарные скобки");
        break;
    }

    default:
        throw TokenException(token, "Неизвестный токен");
    }
}
