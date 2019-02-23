#pragma once

#include <exception>
#include "token.h"

class TokenException : public std::exception
{
public:
    explicit TokenException(const Token &token, const std::string &msg);

public:
    const char *what() const noexcept override;
    Token token() const;

private:
    Token _token;
    std::string _msg;
};

class NoIntersectionException : public std::exception
{
public:
    explicit NoIntersectionException(const std::string &msg);

public:
    const char *what() const noexcept override;

private:
    std::string _msg;
};
