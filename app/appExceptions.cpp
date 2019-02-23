#include "appExceptions.h"

TokenException::TokenException(const Token &token, const std::string &msg)
    : _token(token),
      _msg(msg)
{}

const char *TokenException::what() const noexcept
{
    return _msg.c_str();
}

Token TokenException::token() const
{
    return _token;
}

NoIntersectionException::NoIntersectionException(const std::string &msg)
    : _msg(msg)
{}

const char *NoIntersectionException::what() const noexcept
{
    return _msg.c_str();
}
